#ifndef KODU_SCORING_LETTER_H_
#define KODU_SCORING_LETTER_H_

// C++ Library
#include <set>
#include <string>

// Kodu Library
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {

	class KoduScoringLetter {
	public:
		//! Returns the container with all the score names
		static const std::set<std::string>& getScoringLetterSet();

		//! Initializes the letter container
		static void initialize();

		//! Checks if the letter is valid and usable
		static bool isValidScoringLetter(const std::string&);

	private:
		//! Contains the container of all valid scoring letters
		static std::set<std::string> scoringLetterNames;
		
		DISALLOW_INSTANTIATION(KoduScoringLetter);
	};
}

#endif // KODU_SCORING_LETTER_H_