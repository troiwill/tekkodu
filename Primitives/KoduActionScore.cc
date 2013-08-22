#include "Kodu/Primitives/KoduActionScore.h"

namespace Kodu {

	/*
	const ScoreRequest& KoduActionScore::execute() {
		if (onceEnabled)		// disable this action if "once" modifier is enabled
			actionDisabled = true;
		ScoreKeeper::addScore(value);
		return value;
	}
	*/
	
	int KoduActionScore::changeScore() {
		if (onceEnabled)
			actionHasAlreadyRan = true;
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

	void KoduActionScore::reinitialize() {
		KoduAction::reinitialize();
	}

	void KoduActionScore::printAttrs() {
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
		PRINT_ATTRS("Once enabled", onceEnabled);
		std::cout << std::endl;
	}
} // end of Kodu namespace