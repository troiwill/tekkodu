// INCLUDES
// tekkotsu
#include "DualCoding/VRmixin.h"
using namespace DualCoding;

// tekkodu
#include "Kodu/Primitives/KoduActionMotion.h"
#include "Kodu/Primitives/KoduConditionBump.h"
#include "Kodu/Primitives/PerceptionSearch.h"

namespace Kodu {

/// ================================= Motion Command ================================= ///

    MotionCommand::MotionCommand()
      : targetObject(),
        dx(0),
        da(0),
        forwardSpeed(0),
        turnSpeed(0),
        cmdValid(false)
    { }

    MotionCommand::MotionCommand(const Shape<CylinderData>& kTarget, float fwdSpeed, float trnSpeed)
      : targetObject(kTarget),
        dx(0.0f),
        da(0.0f),
        forwardSpeed(fwdSpeed),
        turnSpeed(trnSpeed),
        cmdValid(true)
    { }

    MotionCommand::MotionCommand(float fwdDist, float trnAngle, float fwdSpeed, float trnSpeed)
      : targetObject(Shape<CylinderData>()),
        dx(fwdDist),
        da(trnAngle),
        forwardSpeed(fwdSpeed),
        turnSpeed(trnSpeed),
        cmdValid(true)
    { }

    MotionCommand::MotionCommand(const DualCoding::Shape<DualCoding::CylinderData>& kTarget,
        float fwdDist, float trnAngle, float fwdSpeed, float trnSpeed)
      : targetObject(kTarget),
        dx(fwdDist),
        da(trnAngle),
        forwardSpeed(fwdSpeed),
        turnSpeed(trnSpeed),
        cmdValid(true)
    { }

    MotionCommand::MotionCommand(const MotionCommand& kCommand)
      : targetObject(kCommand.targetObject),
        dx(kCommand.dx),
        da(kCommand.da),
        forwardSpeed(kCommand.forwardSpeed),
        turnSpeed(kCommand.turnSpeed),
        cmdValid(kCommand.cmdValid)
    { }

    MotionCommand::~MotionCommand() {
        // no explicit implementation
    }

    MotionCommand& MotionCommand::operator=(const MotionCommand& kCommand) {
        if (this != &kCommand) {
            targetObject = kCommand.targetObject;
            dx = kCommand.dx;
            da = kCommand.da;
            forwardSpeed = kCommand.forwardSpeed;
            turnSpeed = kCommand.turnSpeed;
            cmdValid = kCommand.cmdValid;
        }
        return *this;
    }

    bool MotionCommand::operator==(const MotionCommand& kCommand) {
        return (targetObject == kCommand.targetObject || (dx == kCommand.dx && da == kCommand.da));
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

    const Shape<CylinderData>& MotionCommand::getTargetObject() const {
        return targetObject;
    }

/// ================================= Kodu Action Motion ================================= ///

    KoduActionMotion::KoduActionMotion(MotionType_t type, MotionRate_t rate, unsigned int motionMagCount)
      : KoduAction("KoduActionMotion", KoduAction::AT_MOTION, false, false),
        motionType(type),
        motionCmd(),
        angleGen(0,0),
        distGen(0,0),
        directionToFace()
    {
        // motion type is move
        if (type < MT_EMPTY_MOTION_TYPE) {
            motionCmd.turnSpeed = 0.0f;

            if (type == MT_MOVE_WANDER) {
                angleGen.setNumericValues(0, M_PI * 2.0f);
                distGen.setNumericValues(500, 300);
            }
            else if (type == MT_MOVE_FORWARD) {
                distGen.setNumericValues(500, 0);
            }
        }
        // motion type is turn
        else {
            if (type == MT_TURN_LEFT)
                angleGen.setNumericValues(-M_PI, 0);    
            else if (type == MT_TURN_RIGHT)
                angleGen.setNumericValues(M_PI, 0);
        }
        motionCmd.cmdValid = true;
    }

    KoduActionMotion::KoduActionMotion(Direction_t direction, MotionRate_t rate,
        unsigned int motionMagCount)
      : KoduAction("KoduActionMotion", KoduAction::AT_MOTION, false, false),
        motionType(MT_TURN_DIRECTION),
        motionCmd(),
        angleGen(0,0),
        distGen(0,0),
        directionToFace(direction)
    {
        motionCmd.cmdValid = true;
    }

    KoduActionMotion::KoduActionMotion(const KoduActionMotion& kAction)
      : KoduAction(kAction),
        motionType(kAction.motionType),
        motionCmd(kAction.motionCmd),
        angleGen(kAction.angleGen),
        distGen(kAction.distGen),
        directionToFace(kAction.directionToFace)
    { }

    //! Destructor
    KoduActionMotion::~KoduActionMotion() {
        // no explicit implementation
    }

    //! Assignment operator
    KoduActionMotion& KoduActionMotion::operator=(const KoduActionMotion& kAction) {
        if (this != &kAction) {
            KoduAction::operator=(kAction);
            motionType = kAction.motionType;
            motionCmd = kAction.motionCmd;
            angleGen = kAction.angleGen;
            distGen = kAction.distGen;
            directionToFace = kAction.directionToFace;
        }
        return *this;
    }

    const MotionCommand& KoduActionMotion::getMotionCommand() {
        bool cmdIsValid = true;
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
                cmdIsValid = false;
                break;
            }

            // to prevent compiler warnings
            case MT_EMPTY_MOTION_TYPE:
                break;

            // turning to a particular direction
            case MT_TURN_DIRECTION:
            {
                float requestedHeading = 0.0f;
                // calculations assume the robot has recently localized
                if (directionToFace & DT_EAST) {
                    requestedHeading = (1.5f * M_PI);
                } else if (directionToFace & DT_WEST) {
                    requestedHeading = (0.5f * M_PI);
                }

                if (directionToFace & DT_NORTH) {
                    requestedHeading = (2.0f * M_PI);
                } else if (directionToFace & DT_SOUTH) {
                    requestedHeading = (1.0f * M_PI);
                }
                // get the robot's current orientation, and the difference between the requested angle
                // and the current orientation
                float currOrient = VRmixin::theAgent->getOrientation();
                float minTurnAngle = requestedHeading - currOrient;
                // if the difference is greater than pi, do NUMB % PI then multiply by -1
                if (std::fabs(minTurnAngle) > M_PI) {
                    float angleDiff = std::fabs(minTurnAngle) - M_PI;
                    minTurnAngle = (minTurnAngle > 0.0f ? (-1.0f * angleDiff) : angleDiff);
                }

                // prevents the robot from executing the turning action repeatedly due to inaccuracy
                static const float kFiveDegrees = 5.0f * M_PI / 180.0f;

                if (std::fabs(minTurnAngle) < kFiveDegrees)
                    cmdIsValid = false;
                else
                    motionCmd.da = minTurnAngle;
                break;
            }

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
        motionCmd.cmdValid = cmdIsValid;
        return motionCmd;
    }

    KoduActionMotion::MotionType_t KoduActionMotion::getMotionType() const {
        return motionType;
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
