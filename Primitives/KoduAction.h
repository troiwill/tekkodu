#ifndef KODU_ACTION_H_
#define KODU_ACTION_H_

// C++ Library
#include <iostream>
#include <string>

// Kodu Library
#include "Kodu/Primitives/KoduPrimitive.h"
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {
        
    //! Kodu Action (derived from Kodu Behavior)
    class KoduAction : public KoduPrimitive {
    public:
        bool actionHasAlreadyRan;       //!< set true after an action has ran

        //! Constructor
        explicit KoduAction(const std::string& kActionName)
          : KoduPrimitive(kActionName),
            actionHasAlreadyRan(false)
        { }
        
        //! Copy constructor
        explicit KoduAction(const KoduAction& kAction)
          : KoduPrimitive(kAction),
            actionHasAlreadyRan(kAction.actionHasAlreadyRan)
        { }

        //! Destructor
        virtual ~KoduAction () {
            // no explicit implementation
        }
        
        //! Assignment operator
        KoduAction& operator=(const KoduAction& kAction) {
            if (this != &kAction) {
                KoduPrimitive::operator=(kAction);
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
            KoduPrimitive::printAttrs();
            PRINT_ATTRS("Action has ran", actionHasAlreadyRan);
        }
    };
} // end of Kodu namespace

#endif // KODU_ACTION_H_
