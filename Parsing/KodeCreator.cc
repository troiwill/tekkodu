#include "Kodu/Parsing/Parser.h"

namespace Kodu {
		
	bool KodeCreator::createKode(const std::vector<ParsedPage*>& tempPages, std::vector<KoduPage*>& koduPages) {
		// main kode parsing loop
		const std::size_t kTempPagesSize = tempPages.size();
		for (std::size_t pgIndex = 0; pgIndex < kTempPagesSize; pgIndex++) {
			// get the current page
			ParsedPage* tempPage = NULL;

			// ASSERTION: the current page should not be NULL
			PARSER_ASSERT(((tempPage = tempPages[pgIndex]) != NULL),
				errorMessage << "Temp page[" << pgIndex << "] is NULL!");

			// create a Kodu page
			KoduPage* koduPage = NULL;

			// ASSERTION: A new Kodu page was created successfully
			PARSER_ASSERT(((koduPage = new KoduPage(tempPage->getPageNumber())) != NULL),
				errorMessage << "An error occurred while trying to create a new Kodu page\n");
			
			std::cout << "Created a page: PAGE " << koduPage->getPageNumber() << std::endl;
			// create the rules for the current page
			const std::size_t kRuleCount = tempPage->getRuleCount();
			for (std::size_t rIndex = 0; rIndex < kRuleCount; rIndex++) {
				ParsedRule* tempRule = NULL;
				KoduAction* koduAction = NULL;
				KoduCondition* koduCondition = NULL;

				// ASSERTION: the current rule should not be NULL
				PARSER_ASSERT(((tempRule = tempPage->getRuleInPos(rIndex)) != NULL),
					errorMessage << "Temp page[" << pgIndex << "], Temp Rule[" << rIndex
					<< "] is NULL!");
				
				int ruleNumber = tempRule->getRuleNumber();
				int parentNumber = tempRule->getParentNumber();

				// create the Kodu condition
				// ASSERTION: The condition was successfully created
				PARSER_ASSERT(((koduCondition = getConditionKode(tempRule->getConditionPhrase())) != NULL),
					errorMessage << "An error occurred while trying to create the condition for "
					<< "rule " << ruleNumber << ". See above.");
				
				// create the Kodu action
				// ASSERTION: The action was successfully created
				PARSER_ASSERT(((koduAction = getActionKode(tempRule->getActionPhrase())) != NULL),
					errorMessage << "An error occurred while trying to create the action for "
					<< "rule " << ruleNumber << ". See above.");
				
				// create a Kodu rule (object) and add the condition and action to it
				KoduRule* koduRule = NULL;

				// ASSERTION: The Kodu rule was created successfully
				PARSER_ASSERT(((koduRule = new KoduRule(ruleNumber, parentNumber)) != NULL),
					errorMessage << "An error occurred while trying to create a Kodu rule.");

				koduRule->condition = koduCondition;
				koduRule->action = koduAction;
				std::cout << "Created Kodu Rule (" << koduRule->getRuleNumber() << ". " 
						  << koduCondition->getBehaviorType() << " + "
						  << koduAction->getBehaviorType() << ").\n";

				// add the newly created rule to the page
				koduPage->addKoduRule(koduRule);
				std::cout << "Added Kodu Rule " << koduRule->getRuleNumber()
						  << " to Page " << koduPage->getPageNumber() << ".\n";

				// bookkeeping
				tempRule = NULL;
				koduAction = NULL;
				koduCondition = NULL;
				koduRule = NULL;
			}
			// add the newly created Kodu page to the Kodu pages vector
			koduPages.push_back(koduPage);
			std::cout << "Added Kodu Page " << koduPage->getPageNumber() << " to the Pages vector.\n";

			// bookkeeping
			tempPage = NULL;
			koduPage = NULL;
		}
		return true;
	}

	KoduCondition* KodeCreator::getConditionKode(ParsedPhrase* tempCondition) {
		// ASSERTION: The Phrase pointer is not NULL
		PARSER_ASSERT((tempCondition != NULL),
			errorMessage << "The variable that (temporarily) holds the parsed action is NULL!");

		// get the head of the phrase (the condition type)
		std::string conditionStr = tempCondition->getPhraseHead()->getKeywordData();

		// the condition that will be returned
		KoduCondition* condition = NULL;

		// Kodu Condition Always
		if (conditionStr == "always") {
			PARSER_ASSERT(((condition = new KoduConditionAlways()) != NULL),
				errorMessage << "An error occurred while trying to create the Kodu Condition Always.\n");
			// return the condition immediately (there is no need to do any else for the condition)
			std::cout << "Created Always condition...\n";
			return condition;
		}
		
		// get the modifiers for a phrase (only called if the condition is not of type Kodu Conditon Always)
		std::vector<TokenBase*> tempModifiers = tempCondition->getPhraseModifiers();

		// Kodu Condition See
		if (conditionStr == "see") {
			// ASSERTION: the Kodu see condition was succesfully created
			PARSER_ASSERT(((condition = createSeeKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the See condition. "
				<< "See above.");
			std::cout << "Created See condition...\n";
		}

		// Kodu Condition Scored
		else if (conditionStr == "scored") {
			// ASSERTION: the Kodu scored condition was successfully created
			PARSER_ASSERT(((condition = createScoredKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Scored condition. "
				<< "See above.");
			std::cout << "Created Scored condition...\n";
		}
		// Kodu Timer Condition
		else if (conditionStr == "timer") {
			// ASSERTION: the Kodu timer condition was successfully created
			PARSER_ASSERT(((condition = createTimerKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Timer condition. See above.");
			std::cout << "Created Timer condition...\n";
		}
		// The user did not use a recognized condition
		else {
			PARSER_ASSERT((ERROR),
				errorMessage << "The keyword \"" << conditionStr
				<< "\" is not a recognized condition.");
		}
		return condition;
	}

	KoduAction* KodeCreator::getActionKode(ParsedPhrase* tempAction) {
		// ASSERTION: The Phrase pointer is not NULL
		PARSER_ASSERT((tempAction != NULL),
			errorMessage << "The variable that (temporarily) hold the parsed action is NULL!");

		// get the head of the phrase (the action type)
		std::string actionStr = tempAction->getPhraseHead()->getKeywordData();
		
		// the action that will be returned
		KoduAction* action = NULL;

		// Kodu Action Do Nothing
		if (actionStr == "do_nothing") {
			PARSER_ASSERT(((action = new KoduActionDoNothing()) != NULL),
				errorMessage << "An error occurred while trying to create the Do Nothing action.");
			std::cout << "Created Do Nothing action...\n";
			// return the action immediately (there is no need to do any else for the action)
			return action;
		}

		// get the modifiers for a phrase
		std::vector<TokenBase*> tempModifiers = tempAction->getPhraseModifiers();

		// Kodu Action Motion (Move)
		if (actionStr == "move") {
			// ASSERTION: The Kodu motion (move) action was successfully created
			PARSER_ASSERT(((action = createMoveKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Motion (Move) action. "
				<< "See above.");
			std::cout << "Created Move action...\n";
		}
		// Kodu Action Play
		else if (actionStr == "play") {
			// ASSERTION: The Kodu play action was successfully created
			PARSER_ASSERT(((action = createPlayKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Play action. "
				<< "See above.");
			std::cout << "Created Play action...\n";
		}
		// Kodu Action Say
		else if (actionStr == "say") {
			// ASSERTION: The Kodu say action was successfully created
			PARSER_ASSERT(((action = createSayKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Say action. "
				<< "See above.");
			std::cout << "Created Say action...\n";
		}
		// Kodu Action Score/Set Score/Subtract
		else if (actionStr == "score" || actionStr == "set_score" || actionStr == "subtract") {
			// ASSERTION: The Scoring action was successfully created
			PARSER_ASSERT(((action = createScoreKode(actionStr, tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the "
				<< "Score/Set Score/Subtract action. See above.");
			std::cout << "Created Scoring (score | set score | subtract) action...\n";
		}
		// Kodu Action Switch to Page (does not need to be parsed)
		else if (actionStr == "switch_to_page") {
			unsigned int pageNumb = (unsigned int)tempAction->getPhraseModifiers()[0]->getNumericData();
			PARSER_ASSERT(((action = new KoduActionPageSwitch(NumericGenerator(pageNumb, 0))) != NULL),
				errorMessage << "An error occurred while trying to create the Page Switch action.");
			std::cout << "Created Page Switch action...\n";
		}
		// Kodu Action Motion (Turn)
		else if (actionStr == "turn") {
			// ASSERTION: The Kodu motion (turn) action was successfully created
			PARSER_ASSERT(((action = createTurnKode(tempModifiers)) != NULL),
				errorMessage << "An error occurred while trying to create the Motion (Turn) action. "
				<< "See above.");
			std::cout << "Created Turn action...\n";
		}
		// The user did not use a recognized action
		else {
			// ASSERTION: The user did not use a recognized action
			PARSER_ASSERT((ERROR),
				errorMessage << "The keyword \"" << actionStr << "\" is not a recognized action.");
		}
		return action;
	}

	bool KodeCreator::isInequalityOperator(TokenBase* mod) {
		if (mod != NULL && mod->isKeywordToken()
			&& KoduInequalityOperator::isValidInequality(mod->getKeywordData()))
		{
			return true;
		}
		return false;
	}

	bool KodeCreator::isScoreDesignator(TokenBase* mod) {
		if (mod != NULL && mod->isKeywordToken()) {
			std::string keyword = mod->getKeywordData();
			return (ColorTypes::isValidColor(keyword) || KoduScoringLetter::isValidScoringLetter(keyword));
		}
		return false;
	}

	bool KodeCreator::isNumericSpecifier(TokenBase* mod) {
		if (mod != NULL) {
			return ((mod->isNumericToken() || (mod->isKeywordToken() && mod->getKeywordData() == "random")));
		}
		return false;
	}

	bool KodeCreator::numericGenParser(std::vector<TokenBase*>& mods, NumericGenerator& numgen) {
		float constOrModuloDivisor = -1.0f;
		float moduloDivisor = -1.0f;
		bool prevTokenWasNumeric = false;
		bool randomKeywordFound = false;
		bool randomTokenWasFoundBeforeNumericToken = false;

		// loop until the current token is not a numeric or keyword token
		while (!mods.empty() && (mods[0]->isNumericToken() || mods[0]->isKeywordToken())) {
			// check to see if the current token is numeric
			if (mods[0]->isNumericToken()) {
				// ASSERTION: The previous token was not a numeric token
				PARSER_ASSERT((!prevTokenWasNumeric),
					errorMessage << "A numeric token cannot succeed another numeric token. "
					"Token \"" << mods[0]->getNumericData() << "\" caused the error.");
				// check if this is the first numeric token
				if (constOrModuloDivisor < 0.0f) {
					// assign the first constant/upper bound found
					constOrModuloDivisor = mods[0]->getNumericData();
					// used to check if the next token is numeric
					prevTokenWasNumeric = true;
				} else if (moduloDivisor < 0.0f) {
					// set the upper bound
					moduloDivisor = mods[0]->getNumericData();
					prevTokenWasNumeric = true;
				}
			}
			// it has to be a keyword token
			else {
				if (mods[0]->getKeywordData() == "random") {
					// ASSERTION: The random token was not already found
					PARSER_ASSERT((!randomKeywordFound),
						errorMessage << "Only one random modifier is allowed in an action or a condition.");
					// note that the "random" keyword was found
					randomKeywordFound = true;
					// note that the previous token is not numeric
					prevTokenWasNumeric = false;
					// if the random modifier was found before the numeric token, then do the following
					if (constOrModuloDivisor < 0.0f) {
						randomTokenWasFoundBeforeNumericToken = true;
					}
				} else {
					// the keyword is something else which SHOULD signal there is nothing for this
					// parsing function to handle
					break;
				}
			}
			// bookkeeping and prevent infinite looping
			GeneralFncs::destroyPtrInVector(mods, 0);
		}
		// create the numeric request
		// check if the random keyword was found
		if (randomKeywordFound) {
			// if an upper bound was set along with a constant, do the following (pattern: ## random ##)
			if (moduloDivisor >= 0.0f) {
				numgen.setNumericValues(constOrModuloDivisor, moduloDivisor);
			}

			else if (constOrModuloDivisor >= 0.0f) {
				// the random token was found before the numeric token (pattern: random ##)
				if (randomTokenWasFoundBeforeNumericToken) {
					numgen.setNumericValues(0.0f, constOrModuloDivisor);
				}
				// the random token was found after the numeric token (pattern: ## random)
				else {
					numgen.setNumericValues(constOrModuloDivisor, 5.0f);
				}
			}
			// no numeric tokens were found and it is the standalone random modifier (pattern: random)
			else {
				numgen.setNumericValues(0.0f, 5.0f);
			}
		}
		// no random keyword was found, so the number found must only be a numeric constant (pattern: ##)
		else {
			numgen.setNumericValues(constOrModuloDivisor, 0.0f);
		}
		return true;
	}
}