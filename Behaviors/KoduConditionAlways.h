#ifndef KODU_CONDITION_ALWAYS_H_
#define KODU_CONDITION_ALWAYS_H_

// Kodu Library
#include "Kodu/Behaviors/KoduCondition.h"

namespace Kodu {
    
    //! Kodu Condition Always (derived from Kodu Condition)
    class KoduConditionAlways : public KoduCondition {
    public:
        //! Constructor
        KoduConditionAlways()
          : KoduCondition ("KoduConditionAlways")
        { }

        //! Copy constructor
        KoduConditionAlways(const KoduConditionAlways& kCondition)
          : KoduCondition(kCondition)
        { }
        
        //! Destructor
        ~KoduConditionAlways () {
            // no explicit implementation
        }
        
        //! Assignment operator
        KoduConditionAlways& operator=(const KoduConditionAlways& kCondition) {
            if (this != &kCondition) {
                KoduCondition::operator=(kCondition);
            }
            return *this;
        }

        //! Evaluates true (always)
        virtual bool evaluate() {
            return true;
        }
        
        //! Used to reinitialize certain variables (e.g. when switching to another page)
        virtual void reinitialize() {
            KoduCondition::reinitialize();
        }
        
        //! Prints the attributes of a particular instance
        virtual void printAttrs() {
            KoduCondition::printAttrs();
        }
    };
} // end of Kodu namespace

#endif // KODU_CONDITION_ALWAYS_H_
