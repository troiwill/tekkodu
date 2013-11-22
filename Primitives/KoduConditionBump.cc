// Kodu Library
#include "Kodu/KoduWorld.h"
#include "Kodu/Primitives/KoduConditionBump.h"
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {

    const float KoduConditionBump::kMaxDistanceAwayToSenseBump = 40.0f; // millimeters
    //const float KoduConditionBump::kMaxDistInBtwToSenseBump = 100.0f;

    bool KoduConditionBump::evaluate(const KoduWorld& kWorldState) {
        // check the following:
        // 1) the agent visually detected the bump,
        // 2) the referenced object (the object the bump condition will react to) is valid
        // 3) the agent has not moved anywhere (translated the body in the x-direction)
        //    (3) assumes that the agent's centroid measurement is correct.
        static float const kMaxDistAgentCanMoveAway = 20.0f;
        bool rv = (visuallyDetectedBump && refdObject.isValid()
            && distanceFromAgentToPoint(agentLastPosAfterDetection) <= kMaxDistAgentCanMoveAway);
        
        // if the not modifier is enabled, negate the value of the return value (rv)
        if (notModifierEnabled)
            rv = !rv;

        // 
        if ((ObjectKeeper::isValid = rv) == true)
            ObjectKeeper::tempObject = refdObject;

        return rv;
    }

    //******************* temp fix
    bool KoduConditionBump::agentIsNearMatchingObject(const DualCoding::ShapeRoot& kExcludedShape) {
    //*******************
    //bool KoduConditionBump::agentIsNearMatchingObject() {
        DualCoding::Shape<DualCoding::CylinderData> obj;
        // get the closest object that matches what this condition is searching for
        //**************** temp fix
        obj = getClosestObjectMatching(objColor, searchLocation, kExcludedShape);
        //****************
        //obj = getClosestObjectMatching(objColor, searchLocation);
        
        // If there is one valid remaining and it is within some distance to the agent,
        // then the robot will react to that object
        std::cout << "Checking if the agent is near a matching object...";
        if (obj.isValid() && (distanceInBetweenAgentAndObject(obj) <= kMaxDistanceAwayToSenseBump)) {
            std::cout << "it's near!\n";
            refdObject = obj;
            return true;
        } else {
            std::cout << "nada.\n";
            refdObject = ObjectKeeper::invalidObject;
            return false;
        }
    }

    void KoduConditionBump::setVisualBumpDetection(bool bval) {
        visuallyDetectedBump = bval;
        // check if the agent visually detected the bump
        if (visuallyDetectedBump) {
            // if it did, note the last position the robot was at when it visually detected the object
            agentLastPosAfterDetection = DualCoding::VRmixin::theAgent->getCentroid();
            ObjectKeeper::tempObject = refdObject;
        }
    }

    const std::string& KoduConditionBump::getObjectColor() const {
        return objColor;
    }

    const std::string& KoduConditionBump::getObjectType() const {
        return objType;
    }

    const DualCoding::Shape<DualCoding::CylinderData>& KoduConditionBump::getTargetObject() {
        return refdObject;
    }

    bool KoduConditionBump::isSameTypeAs(const KoduPrimitive* kPrimitive) {
        return (dynamic_cast<const KoduConditionBump*>(kPrimitive) != NULL);
    }

    void KoduConditionBump::reinitialize() {
        // TODO (9/Aug/2013)
        // do I need to clear the Shape object?
    }
    
    void KoduConditionBump::printAttrs() const {
        KoduCondition::printAttrs();
        // not enabled?
        PRINT_ATTRS("Not enabled", notModifierEnabled);
        // object color and type
        std::cout << "Object color and type: " << objColor << " " << objType << std::endl;
        // search region
        /*
        std::cout << "Search region:";
        if (searchRegion == SRG_UNRESTRICTED) {
            std::cout << " unrestricted\n";
        } else {
            if (searchRegion & SRG_TO_LEFT) {
                std::cout << " to_left";
            } else if (searchRegion & SRG_TO_RIGHT) {
                std::cout << " to_right";
            }

            if (searchRegion & SRG_IN_FRONT) {
                std::cout << " in_front";
            } else if (searchRegion & SRG_BEHIND) {
                std::cout << " behind";
            }
            std::cout << std::endl;
        }
        */
        // referenced object...
    }
}
