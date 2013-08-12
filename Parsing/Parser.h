#ifndef PARSER_H_
#define PARSER_H_

// C++ Library
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Kodu Library
// Behaviors
// Actions
#include "Kodu/Behaviors/KoduAction.h"
#include "Kodu/Behaviors/KoduActionDoNothing.h"
#include "Kodu/Behaviors/KoduActionMotion.h"
#include "Kodu/Behaviors/KoduActionPageSwitch.h"
#include "Kodu/Behaviors/KoduActionPlay.h"
#include "Kodu/Behaviors/KoduActionSay.h"
#include "Kodu/Behaviors/KoduActionScore.h"
// Conditions
#include "Kodu/Behaviors/KoduCondition.h"
#include "Kodu/Behaviors/KoduConditionAlways.h"
#include "Kodu/Behaviors/KoduConditionSee.h"
#include "Kodu/Behaviors/KoduConditionScored.h"
#include "Kodu/Behaviors/KoduConditionTimer.h"

#include "Kodu/Behaviors/PerceptionSearch.h"
// Designators
#include "Kodu/Designator/ColorTypes.h"
#include "Kodu/Designator/KoduInequalityOperator.h"
#include "Kodu/Designator/KoduScoringLetter.h"
// General Functions
#include "Kodu/General/GeneralFncs.h"
#include "Kodu/General/GeneralMacros.h"
// Generators
#include "Kodu/Generators/KoduGenerators.h"
// Parsing
#include "Kodu/Parsing/ParsedPage.h"
#include "Kodu/Parsing/ParsedPhrase.h"
#include "Kodu/Parsing/ParsedRule.h"
#include "Kodu/Parsing/Token.h"

#include "Kodu/KoduPage.h"
#include "Kodu/KoduRule.h"

namespace Kodu {
	
	#define RED_ERR	"\x1b[31m"
	#define YEL_ERR	"\x1b[33m"
	#define NON_ERR	"\x1b[0m"

	#define ERROR	false

	#define PARSER_ASSERT(CONDITION, ERROR_STATEMENT)							\
		if ((CONDITION) == false) {												\
			std::stringstream errorMessage;										\
			ERROR_STATEMENT;													\
			std::cerr << RED_ERR << errorMessage.str() << NON_ERR << std::endl; \
			return false;														\
		}

	class KodeCreator {
	public:
		//! Takes the parsed tokens and creates a Kodu game
		static bool createKode(const std::vector<ParsedPage*>&, std::vector<KoduPage*>&);

	private:
		//! Used to call the appropriate Kode action parser depending on the Phrase head
		static KoduAction* getActionKode(ParsedPhrase*);

		//! Used to call the appropriate Kode condition parser depending on the Phrase head
		static KoduCondition* getConditionKode(ParsedPhrase*);

		//! Checks if a modifier is an inequality operator
		static bool isInequalityOperator(TokenBase*);

		//! Checks if a modifier is a numeric specifier (random modifier or number(s))
		static bool isNumericSpecifier(TokenBase*);

		//! Checks if a modifier is a score designator
		static bool isScoreDesignator(TokenBase*);

		//! Creates a Numeric Generator from numeric specifiers (random modifier and/or number(s))
		static bool numericGenParser(std::vector<TokenBase*>&, NumericGenerator&);
		
		// action parsers
		//! Creates the move action
		static KoduActionMotion* createMoveKode(std::vector<TokenBase*>&);

		//! Creates the play action
		static KoduActionPlay* createPlayKode(std::vector<TokenBase*>&);
		
		//! Creates the say action
		static KoduActionSay* createSayKode(std::vector<TokenBase*>&);

		//! Creates the score action
		static KoduActionScore* createScoreKode(const std::string&, std::vector<TokenBase*>&);

		//! Creates the move action
		static KoduActionMotion* createTurnKode(std::vector<TokenBase*>&);

		// condition parsers
		//! Creates the see condition
		static KoduConditionSee* createSeeKode(std::vector<TokenBase*>&);

		//! Creates the scored condition
		static KoduConditionScored* createScoredKode(std::vector<TokenBase*>&);
		
		//! Creates the timer condition
		static KoduConditionTimer* createTimerKode(std::vector<TokenBase*>&);

		//! Disallows anyone from creating an instance of this class
		DISALLOW_INSTANTIATION(KodeCreator);
	};

	class TokenParser {
	public:
		//! Parses the retrieved Kodu code into the appropriate data structures
		static bool parseTokens(const std::vector<std::string>&, std::vector<ParsedPage*>&);
	
		//! Reads the Kodu written code from a file
		static bool readText(std::vector<std::string>&);

	private:
		//! Contains all the allowable Kodu "keywords"
		static std::set<std::string> koduKeywords;

		//! Returns the index of the search item
		static int contains(const std::vector<TokenBase*>&, const std::string&);
		
		//! Returns a vector of tokens parsed from a string
		static bool tokenize(const std::string&, std::vector<TokenBase*>&);

		// ==============================
		//! Clear the keyword set (it's no longer needed after parsing)
		static void clearKeywordSet();

		//! Initializes the keyword set
		static void initializeKeywordSet();

		//! Disallows anyone from creating an instance of this class
		DISALLOW_INSTANTIATION(TokenParser);
	};
}

# endif // PARSER_H_