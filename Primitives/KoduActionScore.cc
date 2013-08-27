#include "Kodu/Primitives/KoduActionScore.h"

namespace Kodu {

    int KoduActionScore::changeScore() {
        int rv = 0;
        switch (type) {
            case ST_SCORE:
                rv = ScoreKeeper::addScore(designator, numericGen.getNumericValue());

            case ST_SET_SCORE:
                rv = ScoreKeeper::setScore(designator, numericGen.getNumericValue());

            case ST_SUBTRACT:
                rv = ScoreKeeper::subtractScore(designator, numericGen.getNumericValue());
        }
        return rv;
    }

    bool KoduActionScore::isSameTypeAs(const KoduPrimitive* kPrimitive) {
        return (dynamic_cast<const KoduActionScore*>(kPrimitive) != NULL);
    }

    void KoduActionScore::reinitialize() {
        KoduAction::reinitialize();
    }

    void KoduActionScore::printAttrs() const {
        KoduAction::printAttrs();
        std::cout << "Score operation type: ";
        switch (type) {
            case ST_SCORE:
                std::cout << "score";
                break;

            case ST_SET_SCORE:
                std::cout << "set score";
                break;

            case ST_SUBTRACT:
                std::cout << "subtract";
                break;
        }
        std::cout << std::endl;
        std::cout << "Score designator: " << designator << std::endl;
        numericGen.printAttrs();
        std::cout << std::endl;
    }
} // end of Kodu namespace