// Kodu Library
#include "Kodu/Behaviors/KoduConditionSee.h"
#include "Kodu/General/GeneralMacros.h"

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/VRmixin.h"

namespace Kodu {

    bool KoduConditionSee::evaluate() {
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

        // test if the search radius is unrestricted
        if (searchRadius != SRD_UNRESTRICTED && objects.size() > 0) {
            if (searchRadius == SRD_CLOSE_BY) {
                std::cout << "Checking map for objects close by me\n";
                objects = DualCoding::subset(objects, IsCloseByAgent());
            } else {
                std::cout << "Checking map for objects far away from me\n";
                objects = DualCoding::subset(objects, IsFarAwayFromAgent());
            }
        }
        
        // get the closest object to the agent from the vector of shapes.
        if (objects.size() > 0)
            _refdObject = getClosestObject(objects);

        // If there is one valid remaining, the robot will react to that object
        if (_refdObject.isValid()) {
            std::cout << "Saw a(n) " << getObjectColor() << " " << getObjectType() << "!\n";
            ObjectKeeper::tempObject = _refdObject;
            ObjectKeeper::isValid = true;
            rv = true;
        }
        
        // check if the not modifier is enabled
        if (notModifierEnabled)
            return (!rv);
        else
            return rv;
    }

    const std::string& KoduConditionSee::getObjectColor() const {
        return objColor;
    }

    const std::string& KoduConditionSee::getObjectType() const {
        return objType;
    }

    const DualCoding::Shape<DualCoding::CylinderData>& KoduConditionSee::getTargetObject() {
        return refdObject;
    }

    void KoduConditionSee::reinitialize() {
        // TODO (9/Aug/2013)
        // do I need to clear the Shape object?
    }
    
    void KoduConditionSee::printAttrs() {
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
        // search radius
        std::cout << "Search radius: ";
        switch (searchRadius) {
            case SRD_UNRESTRICTED:
                std::cout << "unrestricted\n";
                break;

            case SRD_CLOSE_BY:
                std::cout << "close_by\n";
                break;

            case SRD_FAR_AWAY:
                std::cout << "far_away\n";
                break;
        }
        // referenced object...
    }
}