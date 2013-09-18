#ifndef KODU_CONDITION_H_
#define KODU_CONDITION_H_

// C++ Library
#include <string>

// Kodu Library
#include "Kodu/Primitives/KoduPrimitive.h"

namespace Kodu {

    //! Kodu Condition (derived from Kodu Primitive)
    class KoduCondition : public KoduPrimitive {
    public:
        enum ConditionTypes {
            CT_ALWAYS = 0,
            CT_BUMP,
            CT_SCORED,
            CT_SEE,
            CT_TIMER
        };

        //! Constructor
        KoduCondition(const std::string& kConditionName, ConditionTypes condType)
          : KoduPrimitive(kConditionName),
            conditionType(condType),
            canEvaluateCondition(true)
        { }
        
        //! Copy constructor
        KoduCondition(const KoduCondition& kCondition)
          : KoduPrimitive(kCondition),
            conditionType(kCondition.conditionType),
            canEvaluateCondition(kCondition.canEvaluateCondition)
        { }

        //! Destructor
        virtual ~KoduCondition() {
            // no explicit implementation
        }

        //! Assignment operator
        KoduCondition& operator=(const KoduCondition& kCondition) {
            if (this != &kCondition) {
                KoduPrimitive::operator=(kCondition);
                conditionType = kCondition.conditionType;
                canEvaluateCondition = kCondition.canEvaluateCondition;
            }
            return *this;
        }
        
        //! Evaluates the event portion of the rule (implementation in derived classes)
        virtual bool evaluate() = 0;
        
        //! Used to reinitialize certain variables during, for example, switching to another page
        virtual void reinitialize() {
            // do nothing
            canEvaluateCondition = true;
        }

        //! Returns the condition type
        ConditionTypes getConditionType() const {
            return conditionType;
        }
        
        //! Prints the attributes of a particular instance
        virtual void printAttrs() const {
            KoduPrimitive::printAttrs();
        }


        bool canEvaluate() const {
            return canEvaluateCondition;
        }

        void disallowEvaluate() {
            canEvaluateCondition = false;
        }

        void allowEvaluate() {
            canEvaluateCondition = true;
        }

    private:
        ConditionTypes conditionType;       //!< the condition type

        bool canEvaluateCondition;
    };
} // end of Kodu namespace

#endif // KODU_CONDITION_H_
