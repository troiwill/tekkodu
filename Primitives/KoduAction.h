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
        //! Constructor
        KoduAction(const std::string& kActionName, bool actionCanUseOnceMod = false, bool runOnce = false)
          : KoduPrimitive(kActionName),
            actionCanRun(true),
            onceModEnabled(actionCanUseOnceMod && runOnce)
        { }
        
        //! Copy constructor
        explicit KoduAction(const KoduAction& kAction)
          : KoduPrimitive(kAction),
            actionCanRun(kAction.actionCanRun),
            onceModEnabled(kAction.onceModEnabled)
        { }

        //! Destructor
        virtual ~KoduAction () {
            // no explicit implementation
        }
        
        //! Assignment operator
        KoduAction& operator=(const KoduAction& kAction) {
            if (this != &kAction) {
                KoduPrimitive::operator=(kAction);
                actionCanRun = kAction.actionCanRun;
                onceModEnabled = kAction.onceModEnabled;
            }
            return *this;
        }

        //! Returns whether or not the action can ran
        bool canRun() const {
            return actionCanRun;
        }

        //! Checks if the once modifier is enabled
        bool onceModIsEnabled() const {
            return onceModEnabled;
        }

        //! Used to reinitialize certain variables (e.g. when switching to another page)
        void reinitialize() {
            actionCanRun = false;
        }

        //! Sets the action can run variable
        void setActionCanRun(bool bVal) {
            actionCanRun = bVal;
        }

        //! Prints the attributes for a particular behavior
        virtual void printAttrs() const {
            KoduPrimitive::printAttrs();
            PRINT_ATTRS("Action can ran", actionCanRun);
            PRINT_ATTRS("Once modifier enabled", onceModEnabled);
        }

    private:
        bool actionCanRun;       //!< states if an action can run
        bool onceModEnabled;     //!< states if the once modifier is enabled (depends on action type too)
    };
} // end of Kodu namespace

#endif // KODU_ACTION_H_
