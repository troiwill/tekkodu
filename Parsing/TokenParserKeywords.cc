// Tekkodu Library
#include "Kodu/Parsing/Parser.h"

// C++ Library
#include <set>

namespace Kodu {
		
	std::set<std::string> TokenParser::koduKeywords;

	void TokenParser::initializeKeywordSet() {
		// add the PAGE, WHEN, and DO identifiers
		koduKeywords.insert("PAGE");
		koduKeywords.insert("WHEN");
		koduKeywords.insert("DO");

		// add the following conditions to the keyword set
		koduKeywords.insert("always");
		koduKeywords.insert("scored");
		koduKeywords.insert("see");
		koduKeywords.insert("timer");

		// add the following actions to the keyword set
		koduKeywords.insert("do_nothing");
		koduKeywords.insert("move");
		koduKeywords.insert("switch_to_page");
		koduKeywords.insert("play");
		koduKeywords.insert("say");
		koduKeywords.insert("score");
		koduKeywords.insert("set_score");
		koduKeywords.insert("subtract");
		koduKeywords.insert("turn");

		// add the following modifiers to the keyword set
		// any modifier
		koduKeywords.insert("any");

		// motion modifiers
		koduKeywords.insert("forward");
		koduKeywords.insert("left");
		koduKeywords.insert("right");
		koduKeywords.insert("towards");
		koduKeywords.insert("wander");
		
		// not modifier
		koduKeywords.insert("not");
		
		// numeric comparison modifiers
		koduKeywords.insert(KoduInequalityOperator::getInequalitySet().begin(),
							KoduInequalityOperator::getInequalitySet().end());

		// object types/names
		koduKeywords.insert("apple");
		koduKeywords.insert("rock");
		koduKeywords.insert("tree");

		// once modifier
		koduKeywords.insert("once");
		
		// random modifier
		koduKeywords.insert("random");

		// rate modifiers
		koduKeywords.insert("quickly");
		koduKeywords.insert("slowly");

		// regional specifiers
		koduKeywords.insert("to_left");
		koduKeywords.insert("to_right");
		koduKeywords.insert("in_front");
		koduKeywords.insert("behind");

		// object distance
		koduKeywords.insert("close_by");
		koduKeywords.insert("far_away");

		// color modifiers
		koduKeywords.insert(ColorTypes::getColorSet().begin(),
							ColorTypes::getColorSet().end());
		
		// score letter modifiers
		koduKeywords.insert(KoduScoringLetter::getScoringLetterSet().begin(),
							KoduScoringLetter::getScoringLetterSet().end());
	}

	void TokenParser::clearKeywordSet() {
		koduKeywords.clear();
	}
}