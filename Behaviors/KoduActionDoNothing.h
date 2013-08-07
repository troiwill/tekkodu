#ifndef KODU_ACTION_DO_NOTHING_H_
#define KODU_ACTION_DO_NOTHING_H_

// Kodu Library
#include "Kodu/Behaviors/KoduAction.h"

namespace Kodu {
	
	class KoduActionDoNothing : public KoduAction {
	public:
		//! Constructor
		KoduActionDoNothing()
		  : KoduAction("KoduActionDoNothing")
		{ }

		//! Copy constructor
		KoduActionDoNothing(const KoduActionDoNothing& kAction)
		  : KoduAction(kAction)
		{ }

		//! Destructor
		~KoduActionDoNothing() {
			// no explicit implementation
		}

		//! Assignment operator
		KoduActionDoNothing& operator=(const KoduActionDoNothing& kAction) {
			if (this != &kAction) {
				KoduAction::operator=(kAction);
			}
			return *this;
		}

		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize() {
			// nothing to do
		}
		
		//! Prints the attributes of a particular instance
		virtual void printAttrs() {
			KoduAction::printAttrs();
		}
	};
}

#endif // KODU_ACTION_DO_NOTHING_H_