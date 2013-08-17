// Tekkodu Library
#include "PerceptionSearch.h"

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
}