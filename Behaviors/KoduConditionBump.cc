// Kodu Library
#include "Kodu/Behaviors/KoduConditionBump.h"
#include "Kodu/General/GeneralMacros.h"

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/VRmixin.h"

namespace Kodu {

    bool KoduConditionBump::evaluate() {
        bool rv = false;
        DualCoding::Shape<DualCoding::CylinderData> _refdObject;
        
        // Tekkotsu function. Returns all the objects that are Cylinders
        NEW_SHAPEVEC(objects, DualCoding::CylinderData,
                        DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));
        
        // Tekkotsu function. Returns all objects with a specified color
        if (objects.size() > 0)
            objects = getObjectsWithColor(objects, objColor);
        
        // test if the search region is unrestricted
        if (searchRegion != SRG_UNRESTRICTED && objects.size() > 0)
            objects = getObjectsLocatedInRegion(objects, searchRegion);

        // get the closest object to the agent from the vector of shapes.
        if (objects.size() > 0)
            _refdObject = getClosestObject(objects);

        // If there is one valid remaining and it is within some distance to the agent,
        // then the robot will react to that object
        if (_refdObject.isValid() && calcDistanceFromAgentToObject(_refdObject) <= 250) {
            std::cout << "Bumped a(n) " << getObjectColor() << " " << getObjectType() << "!\n";
            ObjectKeeper::tempObject = _refdObject;
            ObjectKeeper::isValid = true;
            rv = true;
        }
        
        if (notModifierEnabled)
            return (!rv);
        else
            return rv;
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

    void KoduConditionBump::reinitialize() {
        // TODO (9/Aug/2013)
        // do I need to clear the Shape object?
    }
    
    void KoduConditionBump::printAttrs() {
        KoduCondition::printAttrs();
        // not enabled?
        PRINT_ATTRS("Not enabled", notModifierEnabled);
        // object color and type
        std::cout << "Object color and type: " << objColor << " " << objType << std::endl;
        // search region
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
        // referenced object...
    }
}