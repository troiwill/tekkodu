#ifndef KODU_ACTION_MOTION_H_
#define KODU_ACTION_MOTION_H_

// C++ Library
#include <cmath>

// Kodu Library
#include "Kodu/Behaviors/KoduAction.h"
#include "Kodu/Generators/KoduGenerators.h"
#include "Kodu/Keepers/ObjectKeeper.h"
#include "DualCoding/DualCoding.h"
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/BaseData.h"

namespace Kodu {

	class MotionCommand {
	public:
		//! Constructor
		MotionCommand()
		  : targetObject(),
		  	forwardSpeed(0),
		  	turnSpeed(0),
		  	dx(0),
		  	da(0),
		  	cmdValid(false)
		{ }

		//! Copy Constructor
		MotionCommand(const MotionCommand& kCommand)
		  : targetObject(kCommand.targetObject),
		  	forwardSpeed(kCommand.forwardSpeed),
		  	turnSpeed(kCommand.turnSpeed),
		  	dx(kCommand.dx),
		  	da(kCommand.da),
		  	cmdValid(kCommand.cmdValid)
		{ }

		//! Destructor
		~MotionCommand() { /* no explicit implementation */ }

		//! Assignment operator
		MotionCommand& operator=(const MotionCommand& kCommand) {
			if (this != &kCommand) {
				targetObject = kCommand.targetObject;
				forwardSpeed = kCommand.forwardSpeed;
				turnSpeed = kCommand.turnSpeed;
				dx = kCommand.dx;
				da = kCommand.da;
				cmdValid = kCommand.cmdValid;
			}
			return *this;
		}

		//! Equals operator
		bool operator==(const MotionCommand&);

		//! Not equals operator
		bool operator!=(const MotionCommand&);

		//! Returns the forward speed
		float getForwardSpeed() const;

		//! Returns the turning speed
		float getTurnSpeed() const;

		//! Returns the distance to travel in the x-direction
		float getDistanceToTravel() const;

		//! Returns the angle to turn (in degrees)
		float getTurningAngle() const;

		//! Returns the value of the valid flag
		bool isValid() const;

		//! Sets the valid flag to false
		void invalidate();

		
		bool targetObjectIsValid() const;

		const DualCoding::Shape<DualCoding::CylinderData>& getTargetObject() const;
		

		//! Makes KoduActionMotion a friend of MotionCommand
		friend class KoduActionMotion;

	private:
		DualCoding::Shape<DualCoding::CylinderData> targetObject;
		float forwardSpeed;	//!< How fast the robot should move forward (units: mm/s)
		float turnSpeed;	//!< How fast the robot should turn (units: rads/s)
		float dx;			//!< How far the robot should move forward (units: mm)
		float da;			//!< How much the robot should turn (units: rads)
		bool cmdValid;		//!< States whether a command is valid or not
	};

	//! Kodu Action Motion (derived from Kodu Action)
	class KoduActionMotion : public KoduAction {
	public:
		//! The different types of move and turn movements
		enum MotionType_t {
			MT_MOVE_WANDER			= 1L << 0,
			MT_MOVE_FORWARD			= 1L << 1,
			MT_MOVE_TOWARDS			= 1L << 2,
			MT_EMPTY_MOTION_TYPE	= 1L << 3,	// place holder
			MT_TURN_LEFT			= 1L << 4,
			MT_TURN_RIGHT			= 1L << 5
		};

		//! Helps set the speed of a particular motion type
		enum RateSpecifier_t {
			RS_NORMAL				= 1L << 0,
			RS_QUICKLY				= 1L << 1,
			RS_SLOWLY				= 1L << 2
		};

		//! Constructor
		KoduActionMotion(MotionType_t type, RateSpecifier_t rate, unsigned int motionMagCount)
		  : KoduAction("KoduActionMotion"),
		  	motionType(type),
		  	motionCmd(),
		  	angleGen(0,0),
		  	distGen(0,0)
		{
			// motion type is move
			if (type < MT_EMPTY_MOTION_TYPE) {
				motionCmd.forwardSpeed = 150.0f;
				motionCmd.turnSpeed = 0.0f;

				if (type == MT_MOVE_WANDER) {
					angleGen.setNumericValues(0, M_PI * 2.0f);
					distGen.setNumericValues(500, 300);
					motionCmd.turnSpeed = M_PI / 5.0f;
				}
				else if (type == MT_MOVE_FORWARD) {
					distGen.setNumericValues(500, 0);
				}
				else if (type == MT_MOVE_TOWARDS) {
					motionCmd.turnSpeed = M_PI / 5.0f;
				}
			}
			// motion type is turn
			else {
				motionCmd.forwardSpeed = 0.0f;
				motionCmd.turnSpeed = M_PI / 5.0f;

				if (type == MT_TURN_LEFT)
					angleGen.setNumericValues(-M_PI, 0);	
				else if (type == MT_TURN_RIGHT)
					angleGen.setNumericValues(M_PI, 0);
			}
			motionCmd.cmdValid = true;
		}

		//! Copy constructor
		KoduActionMotion(const KoduActionMotion& kAction)
		  : KoduAction(kAction),
		  	motionType(kAction.motionType),
		  	motionCmd(kAction.motionCmd),
		  	angleGen(kAction.angleGen),
		  	distGen(kAction.distGen)
		{ }

		//! Destructor
		~KoduActionMotion() {
			// no explicit implementation
		}

		//! Assignment operator
		KoduActionMotion& operator=(const KoduActionMotion& kAction) {
			if (this != &kAction) {
				KoduAction::operator=(kAction);
				motionType = kAction.motionType;
				motionCmd = kAction.motionCmd;
				angleGen = kAction.angleGen;
				distGen = kAction.distGen;
			}
			return *this;
		}

		//! Returns the motion command
		const MotionCommand& getMotionCommand();

		//! Returns the motion type
		MotionType_t getMotionType() const;

		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize();

	private:
		MotionType_t motionType;	//!< States what type of movement the robot will do
		MotionCommand motionCmd;	//!< Used to tell the robot what object to go to, how far to move, etc.
		NumericGenerator angleGen;	//!< Used to generate constant or random angle values
		NumericGenerator distGen;	//!< Used to generate constant or random distance values
	};
}

#endif // KODU_ACTION_MOTION_H_