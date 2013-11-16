// INCLUDES
// tekkotsu library
#include "DualCoding/ShapeRoot.h"
#include "Shared/ProjectInterface.h"

// tekkodu library
#include "Kodu/KoduWorld.h"
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Primitives/KoduConditionGot.h"

using namespace DualCoding;

namespace Kodu {

    bool KoduConditionGot::evaluate(const KoduWorld& kWorldState) {
        const DualCoding::ShapeRoot& kGripperObjectRef = kWorldState.thisAgent.gripperObject;

        bool rv = ((kGripperObjectRef.isValid())
                && (kWorldState.thisAgent.targetObjectIsInGripper)
                && (ProjectInterface::getColorName(kGripperObjectRef->getColor()) == objColor));
        
        // if the not modifier is enabled, negate the value of the return value (rv)
        if (notModifierEnabled)
            rv = !rv;

        // 
        if ((ObjectKeeper::isValid = rv) == true) {
            ObjectKeeper::tempObject
                = ShapeRootTypeConst(kWorldState.thisAgent.gripperObject, CylinderData);
        }

        return rv;
    }

    const std::string& KoduConditionGot::getObjectColor() const {
        return objColor;
    }

    const std::string& KoduConditionGot::getObjectType() const {
        return objType;
    }

    //************* temp fix
    const DualCoding::Shape<DualCoding::CylinderData>& KoduConditionGot::getTargetObject() {
        return ObjectKeeper::tempObject;
    }
    //*************

    void KoduConditionGot::reinitialize() {
        // do nothing
    }
    
    void KoduConditionGot::printAttrs() const {
        KoduCondition::printAttrs();
        // not enabled?
        PRINT_ATTRS("Not enabled", notModifierEnabled);
        // object color and type
        std::cout << "Object color and type: " << objColor << " " << objType << std::endl;
    }
}
