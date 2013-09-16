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
        KoduCondition(const std::string& kConditionName)
          : KoduPrimitive(kConditionName),
        { }
        
        //! Copy constructor
        KoduCondition(const KoduCondition& kCondition)
          : KoduPrimitive(kCondition)
        { }

        //! Destructor
        virtual ~KoduCondition() {
            // no explicit implementation
        }

        //! Assignment operator
        KoduCondition& operator=(const KoduCondition& kCondition) {
            if (this != &kCondition) {
                KoduPrimitive::operator=(kCondition);
            }
            return *this;
        }
        
        //! Evaluates the event portion of the rule (implementation in derived classes)
        virtual bool evaluate() = 0;
        
        //! Used to reinitialize certain variables during certain circumstances (e.g. switching to another page)
        virtual void reinitialize() {
            // do nothing
        }
        
        //! Prints the attributes of a particular instance
        virtual void printAttrs() const {
            KoduPrimitive::printAttrs();
        }
    };
} // end of Kodu namespace

#endif // KODU_CONDITION_H_
