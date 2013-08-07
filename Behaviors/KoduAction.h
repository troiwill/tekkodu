#ifndef KODU_ACTION_H_
#define KODU_ACTION_H_

// C++ Library
#include <iostream>
#include <string>

// Kodu Library
#include "Kodu/Behaviors/KoduBehavior.h"
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {
		
	//! Kodu Action (derived from Kodu Behavior)
	class KoduAction : public KoduBehavior {
	public:
		bool actionHasAlreadyRan;		//!< set true after an action has ran

		//! Constructor
		explicit KoduAction(const std::string& kActionName)
		  : KoduBehavior(kActionName),
		  	actionHasAlreadyRan(false)
		{ }
		
		//! Copy constructor
		explicit KoduAction(const KoduAction& kAction)
		  : KoduBehavior(kAction),
		  	actionHasAlreadyRan(kAction.actionHasAlreadyRan)
		{ }

		//! Destructor
		virtual ~KoduAction () {
			// no explicit implementation
		}
		
		//! Assignment operator
		KoduAction& operator=(const KoduAction& kAction) {
			if (this != &kAction) {
				KoduBehavior::operator=(kAction);
				actionHasAlreadyRan = kAction.actionHasAlreadyRan;
			}
			return *this;
		}

		//! Returns whether or not the action has ran
		bool actionHasRan() {
			return actionHasAlreadyRan;
		}

		//! Used to reinitialize certain variables (e.g. when switching to another page)
		void reinitialize() {
			actionHasAlreadyRan = false;
		}

		//! Prints the attributes for a particular behavior
		virtual void printAttrs() {
			KoduBehavior::printAttrs();
			PRINT_ATTRS("Action has ran", actionHasAlreadyRan);
		}
	};
} // end of Kodu namespace

#endif // KODU_ACTION_H_
