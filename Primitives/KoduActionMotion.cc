#include "Kodu/Primitives/KoduActionMotion.h"
#include "Kodu/Primitives/KoduConditionBump.h"
#include "Kodu/Primitives/PerceptionSearch.h"

#include "DualCoding/VRmixin.h"

namespace Kodu {

    bool MotionCommand::operator==(const MotionCommand& kCommand) {
        return (forwardSpeed == kCommand.forwardSpeed
            && turnSpeed == kCommand.turnSpeed
            && dx == kCommand.dx
            && da == kCommand.da);
    }

    bool MotionCommand::operator!=(const MotionCommand& kCommand) {
        return (!(*this == kCommand));
    }

    float MotionCommand::getForwardSpeed() const {
        return forwardSpeed;
    }

    float MotionCommand::getTurnSpeed() const {
        return turnSpeed;
    }

    float MotionCommand::getDistanceToTravel() const {
        return dx;
    }

    float MotionCommand::getTurningAngle() const {
        return da;
    }

    bool MotionCommand::isValid() const {
        return cmdValid;
    }

    void MotionCommand::invalidate() {
        cmdValid = false;
    }

    bool MotionCommand::targetObjectIsValid() const {
        return targetObject.isValid();
    }

    const DualCoding::Shape<DualCoding::CylinderData>& MotionCommand::getTargetObject() const {
        return targetObject;
    }

    const float KoduActionMotion::kWalkingSpeed = 150.0f;

    const MotionCommand& KoduActionMotion::getMotionCommand() {
        switch (motionType) {
            // random walk
            case MT_MOVE_WANDER:
            {
                motionCmd.dx = distGen.getNumericValue();
                motionCmd.da = angleGen.getNumericValue();
                break;
            }
            
            // moving forward
            case MT_MOVE_FORWARD:
            {
                motionCmd.dx = distGen.getNumericValue();
                break;
            }

            // moving towards
            case MT_MOVE_TOWARDS:
            {
                if (ObjectKeeper::isValid) {
                    if (distanceInBetweenAgentAndObject(ObjectKeeper::tempObject)
                        > KoduConditionBump::kMaxDistanceAwayToSenseBump)
                    {
                        motionCmd.targetObject = ObjectKeeper::tempObject;
                        break;
                    }
                }
                motionCmd.targetObject = ObjectKeeper::invalidObject;
                motionCmd.cmdValid = false;
                break;
            }

            // to prevent compiler warnings
            case MT_EMPTY_MOTION_TYPE:
                break;

            // turning left
            case MT_TURN_LEFT:
            {
                motionCmd.da = angleGen.getNumericValue();
                break;
            }

            // turning right
            case MT_TURN_RIGHT:
            {
                motionCmd.da = angleGen.getNumericValue();
                break;
            }
        }
        return motionCmd;
    }

    KoduActionMotion::MotionType_t KoduActionMotion::getMotionType() const {
        return motionType;
    }

    bool KoduActionMotion::isSameTypeAs(const KoduPrimitive* kPrimitive) {
        return (dynamic_cast<const KoduActionMotion*>(kPrimitive) != NULL);
    }

    bool KoduActionMotion::motionTypeIsMove() const {
        return (motionType < MT_EMPTY_MOTION_TYPE);
    }

    bool KoduActionMotion::motionTypeIsTurn() const {
        return (motionType > MT_EMPTY_MOTION_TYPE);
    }

    void KoduActionMotion::reinitialize() {
        KoduAction::reinitialize();
    }
}
