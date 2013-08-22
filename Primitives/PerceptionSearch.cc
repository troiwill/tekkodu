// Tekkodu Library
#include "Kodu/Primitives/PerceptionSearch.h"

namespace Kodu {
    
    bool IsLeftOfAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the bearing from the agent to the shape and return the result
        return (calcBearingFromAgentToObject(kShape) > 0.0f ? true : false);
    }

    bool IsRightOfAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // is right of agent is simply the opposite of what is left of agent would return
        return (!(IsLeftOfAgent()(kShape)));
    }

    bool IsInFrontAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the bearing from the agent to the shape
        AngSignPi dtheta = calcBearingFromAgentToObject(kShape);
        // since the last calculate would produce a value between -pi/2 and +pi/2, add pi/2 to last calculate
        // the result will give a position angle if the object is in front the agent
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
        return (calcDistanceFromAgentToObject(kShape) <= 700.0f);
    }

    bool IsFarAwayFromAgent::operator()(const DualCoding::ShapeRoot& kShape) const {
        // get the distance between the shape and the agent
        return (calcDistanceFromAgentToObject(kShape) >= 1050.0f);
    }

    DualCoding::Shape<DualCoding::CylinderData> getClosestObjectMatching(const std::string& color,
                                                                         SearchLocation_t location)
    {
        // The closest object that matches the specified criteria
        DualCoding::Shape<DualCoding::CylinderData> closestMatch;

        // Tekkotsu function. Returns all the objects that are Cylinders
        NEW_SHAPEVEC(matchingObjects, DualCoding::CylinderData,
                     DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));

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