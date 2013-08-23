#include "Kodu/Primitives/KoduActionSay.h"

namespace Kodu {

    /*
    const SayRequest& KoduActionSay::execute() {
        if (onceEnabled)        // disable this action if "once" modifier is enabled
            actionDisabled = true;
        switch (sayreq.seq) {
            case SayRequest::SO_RANDOM:
            {
                int randomInt = rand() % sayreq.strings.size();
                sayreq.text = sayreq.strings[randomInt];
                break;
            }

            default:
                sayreq.text = sayreq.strings[vecIndex];
                vecIndex = (vecIndex + 1) % sayreq.strings.size();
                break;
        }
        
        // ==== for debugging ==== //
        std::cout << getBehaviorType() << ": \"" << sayreq.text << "\" ["
                  << KoduColor::getColorName(sayreq.colorIndex) << "]\n";
        // ======================= //
        return sayreq;
    }
    */

    const std::string& KoduActionSay::getSpeechText() {
        //if (onceEnabled)
        //  actionHasAlreadyRan = true;
        return literalGen.getLiteralString();
    }
    
    void KoduActionSay::reinitialize() {
        KoduAction::reinitialize();
    }

    void KoduActionSay::printAttrs() {
        KoduAction::printAttrs();
        std::cout << "Speech designator: " << designator << std::endl;
        literalGen.printAttrs();
        PRINT_ATTRS("Once enabled", onceEnabled);
        std::cout << std::endl;
    }
}