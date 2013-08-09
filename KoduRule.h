#ifndef KODU_RULE_H_
#define KODU_RULE_H_

// Kodu Library
#include "Kodu/Behaviors/KoduAction.h"
#include "Kodu/Behaviors/KoduCondition.h"

namespace Kodu {
	//! Kodu Rule
	class KoduRule {
	public:
		unsigned int ruleNumber;		//!< Rule number
		unsigned int parent;			//!< Parent rule (assigned during program parsing)
		KoduCondition* condition;		//!< Condition to evaluate
		KoduAction* action;				//!< Action to execute
		bool condEvalResult;			//!< Evaluation result for a condition (true|false)
		
		//! Constructor
		KoduRule(unsigned int kRuleNumber, unsigned int kParent)
			: ruleNumber(kRuleNumber),
			parent(kParent),
			condition(NULL),
			action(NULL),
			condEvalResult(false)
		{ }
		
		//! Destructor
		~KoduRule() {
			delete condition;
			delete action;
		}
		
		//! Get the rule number
		unsigned int getRuleNumber() const {
			return ruleNumber;
		}

		//! Get the parent number
		unsigned int getParentNumber() const {
			return parent;
		}

		//! Checks if a rule has a parent (if it is indented)
		bool isIndented() const {
			return (getParentNumber() > 0);
		}

		//! Reinitializes particular variables in the condition and action of a rule
		void reinitializePrimitives() {
			condEvalResult = false;
			condition->reinitialize();
			action->reinitialize();
		}
		
	private:
		DISALLOW_COPY_ASSIGN(KoduRule);
	};
}
#endif // KODU_RULE_H_