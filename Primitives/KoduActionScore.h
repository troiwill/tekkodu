#ifndef KODU_ACTION_SCORE_H_
#define KODU_ACTION_SCORE_H_

// C++ Library
#include <iostream>

// Kodu Library
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Generators/KoduGenerators.h"
#include "Kodu/Keepers/ScoreKeeper.h"
#include "Kodu/Primitives/KoduAction.h"

/**
 * NAME:            KODU ACTION SCORE
 * TYPE:            ACTION
 * KODU EQUIVALENT: SCORE
 * DESCRIPTION:
 *      This class is used to add, subtract, and set a particular score using a key (score designator).
**/

namespace Kodu {

    //! Kodu Action Score (derived from Kodu Action)
    class KoduActionScore : public KoduAction {
    public:
        enum ScoringType_t {
            ST_SCORE = 0,
            ST_SET_SCORE,
            ST_SUBTRACT
        };

        //! Constructor
        KoduActionScore(ScoringType_t scoreOpType, const NumericGenerator& kNumericGen,
            const std::string& kDesignator, bool useOnce)
          : KoduAction("KoduActionScore"),
            type(scoreOpType),
            numericGen(kNumericGen),
            designator(kDesignator),
            onceEnabled(useOnce)
        {
            ScoreKeeper::registerScore(designator);
        }

        //! Copy constructor
        KoduActionScore(const KoduActionScore& kAction)
          : KoduAction(kAction),
            type(kAction.type),
            numericGen(kAction.numericGen),
            designator(kAction.designator),
            onceEnabled(kAction.onceEnabled)
        { }
        
        //! Destructor
        ~KoduActionScore() {
            // no explicit implementation
        }
        
        //! Assignment operator
        KoduActionScore& operator=(const KoduActionScore& kAction) {
            if (this == &kAction) {
                KoduAction::operator=(kAction);
                type = kAction.type;
                numericGen = kAction.numericGen;
                designator = kAction.designator;
                onceEnabled = kAction.onceEnabled;
            }
            return *this;
        }

        //! Changes the score based on the operation type
        int changeScore();

        //! Used to reinitialize certain variables (e.g. when switching to another page)
        virtual void reinitialize();

        //! Prints the attributes of a particular instance
        virtual void printAttrs();

    private:
        ScoringType_t type;             //!< States whether a score is incremented, decremented, or set
        NumericGenerator numericGen;    //!< Generates a numeric value
        std::string designator;         //!< The score designator
        bool onceEnabled;               //!< States if an action should run once every time a condition is evaluated true
    };
}

#endif // KODU_ACTION_SCORE_H_