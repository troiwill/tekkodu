// Tekkodu Library
#include "Kodu/Primitives/PerceptionSearch.h"

using namespace DualCoding;

namespace Kodu {

    bool HasAreaGreaterThan::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the shape's bounding box
        BoundingBox2D shapeBox = kShape->getBoundingBox();
        // calculate the differences of max and min's x and y values
        float xLength = shapeBox.max[0] - shapeBox.min[0];
        float yLength = shapeBox.max[1] - shapeBox.min[1];
        // get the approximate area of the shape
        float shapeArea = xLength * yLength;
        return (shapeArea > minAcceptableShapeArea);
    }

    bool IsMatchForTargetObject::operator()(const DualCoding::ShapeRoot& kShape) const {
        // check if the objects match each other
        return (targetObject->isMatchFor(kShape));
    }

/*
    bool IsNotStar::operator()(const DualCoding::ShapeRoot& kWShape) const {
        return (!(kWShape->getType() && ShapeRootTypeConst(kWShape, AprilTagData)->getTagID() <= 4));
    }
*/
    bool IsNotWorldShape::operator()(const ShapeRoot& kWShape) const {
        switch (kWShape->getType()) {
            // if it's a cylinder, it must have a radius within some range
            case cylinderDataType:
            {
                const Shape<CylinderData>& kCyl = ShapeRootTypeConst(kWShape, CylinderData);
                if ((kCyl->getRadius() < 40.0f) || (kCyl->getRadius() > 70.0f))
                    return true;
                break;
            }

            // if it's an april tag, it must be the north star
            case aprilTagDataType:
            {
                //const Shape<AprilTagData>& kTag = ShapeRootTypeConst(kWShape, AprilTagData);
                //if (kTag->getTagID() > 4)
                //    return true;
                //break;
                return IsStar()(kWShape);
            }

            // the following should not be included in the vector
            case agentDataType:
            case localizationParticleDataType:
            case polygonDataType:
                return false;

            default:
                return true;
        }
        return false;
    }

    bool IsStar::operator()(const DualCoding::ShapeRoot& kWShape) const {
        return (kWShape.isValid() && kWShape->getType() == aprilTagDataType
            && ShapeRootTypeConst(kWShape, AprilTagData)->getTagID() <= 4);
    }

    bool IsLandmark::operator()(const DualCoding::ShapeRoot& kShape) const {
        return ((kShape->getType() == DualCoding::cylinderDataType)
             && (ProjectInterface::getColorName(kShape->getColor()) == std::string("green")));
    }

    bool IsShapeOfType::operator()(const DualCoding::ShapeRoot& kShape) const {
        return (kShape->getType() == targetShapeType);
    }

    //********* temp fix
    bool IsNotExcludedShape::operator()(const DualCoding::ShapeRoot& kShape) const {
        return (excludedShape.getId() != kShape.getId());
    }
    //**********
    
    bool IsLeftOfAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the bearing from the agent to the shape and return the result
        return (bearingFromAgentToObject(kShape) > 0.0f ? true : false);
    }

    bool IsRightOfAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // is right of agent is simply the opposite of what is left of agent would return
        return (!(IsLeftOfAgent()(kShape)));
    }

    bool IsInFrontAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the bearing from the agent to the shape
        AngSignPi dtheta = bearingFromAgentToObject(kShape);
        // since the last calculation would produce a value between -pi/2 and +pi/2,
        // add pi/2 to the last calculation. the result will give a position angle
        // if the object is in front the agent
        dtheta += AngSignPi(M_PI / 2.0f);
        // check the value of theta
        return (dtheta > 0.0f ? true : false);
    }

    bool IsBehindAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // is behind agent is simply the opposite of what is in front agent would return
        return (!(IsInFrontAgent()(kShape)));
    }

    bool IsCloseByAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the distance between the shape and the agent
        return (distanceFromAgentToObject(kShape) <= 700.0f);
    }

    bool IsFarAwayFromAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the distance between the shape and the agent
        return (distanceFromAgentToObject(kShape) >= 1050.0f);
    }

    float bearingFromAgentToPoint(const DualCoding::Point& kPoint) {
        float dtheta = 0.0f;
        switch (kPoint.getRefFrameType()) {
            // take the agent's centroid into consideration if the point is allocentric
            case DualCoding::allocentric:
            {
                // calculate the bearing between some point "kPoint" and the agent's position
                // bearing2ThisPoint = arctan(dy/dx)
                const DualCoding::Point& kAgentPt = DualCoding::VRmixin::theAgent->getCentroid();
                AngSignPi bearing2ThisPoint = (kPoint - kAgentPt).atanYX();
                // subtract the agent's orientation (heading) from the bearing to get the point's angle
                // relative ot the agent
                dtheta = bearing2ThisPoint - AngSignPi(DualCoding::VRmixin::theAgent->getOrientation());
                break;
            }
            // simply calculate the arctan of the point...
            case DualCoding::egocentric:
            {
                dtheta = kPoint.atanYX();
                break;
            }
            // handles all other Reference Frame Types...
            default:
                std::cout << "WARNING: Used unhandled reference frame in bearingFromAgentToPoint(...)\n";
                break;
        }
        // return the angle between the agent and the target point
        return dtheta;
    }

    float bearingFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
        return bearingFromAgentToPoint(kShape->getCentroid());
    }

    float distanceFromAgentToPoint(const DualCoding::Point& kPoint) {
        float dx = kPoint.coordX();
        float dy = kPoint.coordY();
        switch (kPoint.getRefFrameType()) {
            // since the point is allocentric, take the agent's centroid into consideration.
            case DualCoding::allocentric:
            {
                // get the agent's point
                DualCoding::Point agentPoint = DualCoding::VRmixin::theAgent->getCentroid();
                // calculate the differences in the shape's and agent's positions
                dx = dx - agentPoint.coordX();
                dy = dy - agentPoint.coordY();
                break;
            }
            // since the point is egocentric, there is nothing more to calculate (the agent's centroid
            // is { 0, 0 }).
            case DualCoding::egocentric:
                break;

            // this handles all other Reference Frame Types...
            default:
                std::cout << "WARNING: Used unhandled reference frame in distanceFromAgentToPoint(...)\n";
                return (0.0f);
        }
        // return the distance
        return sqrt((dx * dx) + (dy * dy));
    }

    float distanceFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
        return distanceFromAgentToPoint(kShape->getCentroid());
    }

    float distanceInBetweenAgentAndObject(const DualCoding::ShapeRoot& kShape) {
        static float const kRobotInflatedRadius = 205.0f;
        float distBtwObjects = 0.0f;
        switch(kShape->getType()) {
            case DualCoding::cylinderDataType:
            {
                // get the radius of the cylinder
                float radius = objectRadius(kShape);
                // calculate the distance between the objects
                float distBetweenCentroids = distanceFromAgentToObject(kShape);
                distBtwObjects = distBetweenCentroids - kRobotInflatedRadius - radius;
                break;
            }
            default:
                std::cout << "WARNING: Used unhandled shape type in "
                          << "distanceInBetweenAgentAndObject(...)\n";
                return (0.0f);
        }
        return (distBtwObjects > 0.0f ? distBtwObjects : 0.0f);
    }

    float objectRadius(const DualCoding::ShapeRoot& kShape) {
        static float const kErrValue = -1.0f;
        float radius = 0.0f;
        switch(kShape->getType()) {
            case DualCoding::cylinderDataType:
                radius = ShapeRootTypeConst(kShape, DualCoding::CylinderData)->getRadius();
                break;

            default:
                std::cout << "WARNING: Used unhandled shape type in objectRadius(...)\n";
                return kErrValue;
        }
        return radius;
    }

    template<typename Type>
    Type getClosestObject(const std::vector<Type>& kObjects)
    {
        const std::size_t kSize = kObjects.size();
        // if the vector's size is zero, return an invalid shape
        if (kSize == 0) {
            return Type();
        }

        // else if the vector's size is one, return the first element (the only shape in the vector)
        else if (kSize == 1) {
            return kObjects[0];
        }

        // else iterate over the vector and find the closest shape
        else {
            Type nearestObject = kObjects[0];
            float nearestObjectDist = distanceFromAgentToObject(nearestObject);
            
            // iterate over the remainder of the objects
            for (std::size_t index = 1; index < kSize; index++) {
                float currentObjectDist = distanceFromAgentToObject(kObjects[index]);
                if (currentObjectDist < nearestObjectDist) {
                    nearestObjectDist = currentObjectDist;
                    nearestObject = kObjects[index];
                }
            }
            return nearestObject;
        }
    }

    template<typename Type>
    std::vector<Type> getObjectsLocated(const std::vector<Type>& kObjects, SearchLocation_t location) {
        // copy the objects to result
        std::vector<Type> result = kObjects;
        std::cout << "Checking map for objects:";
        
        // test if the search location should be limited to the front or back
        if (location & SL_IN_FRONT) {
            std::cout << " [in front]";
            result = DualCoding::subset(result, IsInFrontAgent());
        } else if (location & SL_BEHIND) {
            std::cout << " [behind]";
            result = DualCoding::subset(result, IsBehindAgent());
        }
            
        // test if the search location should be limited to the left or right sides
        if (location & SL_TO_LEFT) {
            std::cout << " [to the left]";
            result = DualCoding::subset(result, IsLeftOfAgent());
        } else if (location & SL_TO_RIGHT) {
            std::cout << " [to the right]";
            result = DualCoding::subset(result, IsRightOfAgent());
        }

        // test if the search location is limited to "close by" or "far away (from)" the agent
        if (location & SL_CLOSE_BY) {
            std::cout << " [close by]";
            result = DualCoding::subset(result, IsCloseByAgent());
        }
        else if (location & SL_FAR_AWAY) {
            std::cout << " [far away]";
            result = DualCoding::subset(result, IsFarAwayFromAgent());
        }
        std::cout << std::endl;
        return result;
    }

    //************* temp fix
    DualCoding::Shape<DualCoding::CylinderData> getClosestObjectMatching(const std::string& color,
        SearchLocation_t location, const DualCoding::ShapeRoot& kExcludedShape)
    //*************
    /*
    DualCoding::Shape<DualCoding::CylinderData> getClosestObjectMatching(const std::string& color,
        SearchLocation_t location, const DualCoding::ShapeRoot& kExcludedShape = DualCoding::ShapeRoot())
    */
    {
        // The closest object that matches the specified criteria
        // (if not assigned a value it is invalid)
        DualCoding::Shape<DualCoding::CylinderData> closestMatch;

        // Tekkotsu function. Returns all the objects that are Cylinders
        NEW_SHAPEVEC(matchingObjects, DualCoding::CylinderData,
                     DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));

        //********** temp fix
        if (matchingObjects.size() > 0 && kExcludedShape.isValid()) {
            matchingObjects = DualCoding::subset(matchingObjects, IsNotExcludedShape(kExcludedShape));
        }
        //**********

        // The following attempts to remove any shapes that may create a false positive
        // (e.g. light reflections on an object (noise) that are perceived as red blobs or canisters)
        if (matchingObjects.size() > 0) {
            matchingObjects = DualCoding::subset(matchingObjects, HasAreaGreaterThan(9000.0f));
        }

        // get objects with a particular color
        if (matchingObjects.size() > 0) {
            matchingObjects = getObjectsWithColor(matchingObjects, color);
        }

        // get objects that are located in a particular region (left, right, front, behind) and
        // distance (close by, far away) from the agent
        if (matchingObjects.size() > 0 && (location != SL_UNRESTRICTED)) {
            matchingObjects = getObjectsLocated(matchingObjects, location);
        }

        // get the closest object to the agent
        if (matchingObjects.size() > 0) {
            closestMatch = getClosestObject(matchingObjects);
        }
        // return the object that matches the search criteria (may be valid or invalid)
        return closestMatch;
    }
}
