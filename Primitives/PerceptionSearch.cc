// Tekkodu Library
#include "Kodu/Primitives/PerceptionSearch.h"

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
        // since the last calculate would produce a value between -pi/2 and +pi/2,
        // add pi/2 to last calculate the result will give a position angle if the object is
        // in front the agent
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

    DualCoding::Shape<DualCoding::CylinderData> getClosestObjectMatching(const std::string& color,
                                                                         SearchLocation_t location)
    {
        // The closest object that matches the specified criteria
        // (if not assigned a value it is invalid)
        DualCoding::Shape<DualCoding::CylinderData> closestMatch;

        // Tekkotsu function. Returns all the objects that are Cylinders
        NEW_SHAPEVEC(matchingObjects, DualCoding::CylinderData,
                     DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));

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
