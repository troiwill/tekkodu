#ifndef KODU_CONDITION_H_
#define KODU_CONDITION_H_

// C++ Library
#include <string>

// Kodu Library
#include "Kodu/Behaviors/KoduBehavior.h"

namespace Kodu {

	//! Kodu Condition (derived from Kodu Behavior)
	class KoduCondition : public KoduBehavior {
	public:
		//! Constructor
		KoduCondition(const std::string& kConditionName)
			: KoduBehavior(kConditionName)
		{ }
		
		//! Copy constructor
		KoduCondition(const KoduCondition& kCondition)
		  : KoduBehavior(kCondition)
		{ }

		//! Destructor
		virtual ~KoduCondition() {
			// no explicit implementation
		}

		//! Assignment operator
		KoduCondition& operator=(const KoduCondition& kCondition) {
			if (this != &kCondition) {
				KoduBehavior::operator=(kCondition);
			}
			return *this;
		}
		
		//! Evaluates the event portion of the rule (implementation in derived classes)
		virtual bool evaluate() = 0;
		
		//! Used to reinitialize certain variables because of certain circumstances (e.g. switching to another page)
		virtual void reinitialize() {
			// do nothing
		}
		
		//! Prints the attributes of a particular instance
		virtual void printAttrs() {
			KoduBehavior::printAttrs();
		}
	};
} // end of Kodu namespace

#endif // KODU_CONDITION_H_