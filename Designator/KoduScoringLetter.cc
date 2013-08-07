#include "KoduScoringLetter.h"

namespace Kodu {
	std::set<std::string> KoduScoringLetter::scoringLetterNames;

	const std::set<std::string>& KoduScoringLetter::getScoringLetterSet() {
		return scoringLetterNames;
	}

	void KoduScoringLetter::initialize() {
		std::string scoreName = "score_?";
		for (int i = 65; i <= 90; i++) {
			scoreName[scoreName.size() - 1] = (char)i;
			scoringLetterNames.insert(scoreName);
		}
		return;
	}

	bool KoduScoringLetter::isValidScoringLetter(const std::string& scoringLetterName) {
		return (bool)scoringLetterNames.count(scoringLetterName);
	}
}