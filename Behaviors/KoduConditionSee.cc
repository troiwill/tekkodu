// Kodu Library
#include "Kodu/Behaviors/KoduConditionSee.h"

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/VRmixin.h"

// Standard C++ Library
#include <algorithm>

namespace Kodu {

    bool KoduConditionSee::evaluate() {
        DualCoding::Shape<DualCoding::CylinderData> _refdObject;
        // Tekkotsu. Where the robot (presumably) is in the world.
        DualCoding::Shape<DualCoding::AgentData> agent = DualCoding::VRmixin::theAgent;
        DualCoding::Point agentLocation = agent.getData().getCentroid();
        
        // Tekkotsu function. Returns all the objects that are Cylinders
        NEW_SHAPEVEC(objects, DualCoding::CylinderData,
                        DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));
        std::cout << "Size of objects vec: " << objects.size() << std::endl;

        // test if any specified objects were seen
        if (objects.size() == 0)
            return false;

        // Tekkotsu function. Returns all objects with a specified color
        objects = DualCoding::subset(objects, DualCoding::IsColor(objColor));
        
        // test if the search region is unrestricted
        if (searchRegion != SRG_UNRESTRICTED && objects.size() > 0) {
            // test if the search area should be limited to the left or right sides
            if (searchRegion & SRG_TO_LEFT) {
                std::cout << "Checking map for objects to the left of me\n";
                objects = DualCoding::subset(objects, IsLeftOfAgent());
            } else if (searchRegion & SRG_TO_RIGHT) {
                std::cout << "Checking map for objects to the right of me\n";
                objects = DualCoding::subset(objects, IsRightOfAgent());
            }

            // test if the search area should be limited to the front or back
            if (searchRegion & SRG_IN_FRONT) {
                std::cout << "Checking map for objects in front me\n";
                objects = DualCoding::subset(objects, IsInFrontAgent());
            } else if (searchRegion & SRG_BEHIND) {
                std::cout << "Checking map for objects behind me\n";
                objects = DualCoding::subset(objects, IsBehindAgent());
            }
        }

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
        
        // units = millimeters.
        float smallestDistance = 200000;
        // Tekkotsu macro for iterating over a vector of Shapes
        SHAPEVEC_ITERATE(objects, DualCoding::CylinderData, currObject)
            float currDistance = agentLocation.xyDistanceFrom(currObject.getData().getCentroid());
            if (currDistance < smallestDistance) {
                smallestDistance = currDistance;
                _refdObject = currObject;
            }
        END_ITERATE;
        
        // Reports if there is at least one valid object after all tests have been performed (above)
        std::cout << "Refd object is" << (_refdObject.isValid() ? " " : " not ") << "valid.\n";
        // If there is one valid remaining, the robot will react to that object
        if (_refdObject.isValid()) {
            ObjectKeeper::tempObject = _refdObject;
            ObjectKeeper::isValid = true;
            return true;
        }
        return false;
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

    }
}