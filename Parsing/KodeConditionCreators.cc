#include "Kodu/Parsing/Parser.h"

namespace Kodu {

    KoduConditionBump* Parser::KodeCreator::createBumpKode(std::vector<TokenBase*>& mods) {
        // mandatory modifiers
        std::string objectType;
        std::string objectColor;

        // optional modifiers
        SearchRegion_t leftRight = SRG_UNRESTRICTED;
        SearchRegion_t frontBack = SRG_UNRESTRICTED;
        bool notEnabled = false;
        
        // checkers
        int tokenCount = 1;
        
        // parsing loop
        while (!mods.empty()) {
            // check if the token is a keyword
            if (mods[0]->isKeywordToken()) {
                std::string keyword = mods[0]->getKeywordData();
                // check if keyword is an object type
                if (keyword == "apple" || keyword == "rock" || keyword == "tree") {
                    // ASSERTION: The object type was not already specifieds
                    PARSER_ASSERT((objectType.empty()),
                        errorMessage << "The bump condition only accepts one object type.\n"
                        << "Previous object found: " << objectType);
                    objectType = keyword;
                }
                // check if the keyword is a color
                else if (Parser::isValidColor(keyword)) {
                    // ASSERTION: The color was not already specified
                    PARSER_ASSERT((objectColor.empty()),
                        errorMessage << "The bump condition only accepts one color.\n"
                        << "Previous color found: " << objectColor);
                    objectColor = keyword;
                }
                // check if the keyword is a regional specifier
                else if (keyword == "in_front" || keyword == "behind") {
                    // ASSERTION: The front/back region was not specified
                    PARSER_ASSERT((frontBack == 0),
                        errorMessage << "A front/back regional specifier was already specified.\n"
                        <<  "Second instance found: " << keyword);
                    if (keyword == "in_front")
                        frontBack = SRG_IN_FRONT;
                    else
                        frontBack = SRG_BEHIND;
                }
                // check if the keyword is a regional specifier
                else if (keyword == "to_left" || keyword == "to_right") {
                    // ASSERTION: The front/back region was not specified
                    PARSER_ASSERT((leftRight == 0),
                        errorMessage << "A front/back regional specifier was already specified.\n"
                        <<  "Second instance found: " << keyword);
                    if (keyword == "to_left")
                        leftRight = SRG_TO_LEFT;
                    else
                        leftRight = SRG_TO_RIGHT;
                }
                // check if the keyword is the "not" modifier
                else if (keyword == "not") {
                    // ASSERTION: The "not" modifier has not been specified before in this condition
                    PARSER_ASSERT((notEnabled == false),
                        errorMessage << "The \"not\" modifier was already used in this rule. It can " << 
                        "only be used once per rule.");
                    notEnabled = true;
                }
                // the user specified the wrong keyword
                else {
                    // ASSERTION: The user entered a wrong keyword
                    PARSER_ASSERT((ERROR),
                        errorMessage << "The keyword \"" << keyword << "\" cannot be used with the "
                        << "Bump condition.");
                }
                // bookkeeping
                GeneralFncs::destroyPtrInVector(mods, 0);
            }
            // the user something other than a keyword and is therefore illegal
            else {
                PARSER_ASSERT((ERROR),
                    errorMessage << "Token " << tokenCount << " cannot be used with the Bump condition.");
            }
            // increment the token counter
            tokenCount++;
        }
        // ASSERTION: the user specified a object type
        PARSER_ASSERT((!objectType.empty()),
            errorMessage << "An object type must be specified (e.g. tree, apple, or rock).");

        // ASSERTION: the user specified a object color
        PARSER_ASSERT((!objectColor.empty()),
            errorMessage << "An object color must be specified (e.g. red, green, blue).");

        // create the condition
        return (new KoduConditionBump(notEnabled, objectType, objectColor, (frontBack | leftRight)));
    }

    KoduConditionSee* Parser::KodeCreator::createSeeKode(std::vector<TokenBase*>& mods) {
        // mandatory modifiers
        std::string objectType;
        std::string objectColor;

        // optional modifiers
        SearchRegion_t leftRight = SRG_UNRESTRICTED;
        SearchRegion_t frontBack = SRG_UNRESTRICTED;
        KoduConditionSee::SearchRadius_t spatialSearch = KoduConditionSee::SRD_UNRESTRICTED;

        // checkers
        int tokenCount = 1;
        
        // parsing loop
        while (!mods.empty()) {
            // check if the token is a keyword
            if (mods[0]->isKeywordToken()) {
                std::string keyword = mods[0]->getKeywordData();
                // check if keyword is an object type
                if (keyword == "apple" || keyword == "rock" || keyword == "tree") {
                    // ASSERTION: The object type was not already specifieds
                    PARSER_ASSERT((objectType.empty()),
                        errorMessage << "The see condition only accepts one object type.\n"
                        << "Previous object found: " << objectType);
                    objectType = keyword;
                }
                // check if the keyword is a color
                else if (Parser::isValidColor(keyword)) {
                    // ASSERTION: The color was not already specified
                    PARSER_ASSERT((objectColor.empty()),
                        errorMessage << "The see condition only accepts one color.\n"
                        << "Previous color found: " << objectColor);
                    objectColor = keyword;
                }
                // check if the keyword is a regional specifier
                else if (keyword == "in_front" || keyword == "behind") {
                    // ASSERTION: The front/back region was not specified
                    PARSER_ASSERT((frontBack == 0),
                        errorMessage << "A front/back regional specifier was already specified.\n"
                        <<  "Second instance found: " << keyword);
                    if (keyword == "in_front")
                        frontBack = SRG_IN_FRONT;
                    else
                        frontBack = SRG_BEHIND;
                }
                // check if the keyword is a regional specifier
                else if (keyword == "to_left" || keyword == "to_right") {
                    // ASSERTION: The front/back region was not specified
                    PARSER_ASSERT((leftRight == 0),
                        errorMessage << "A front/back regional specifier was already specified.\n"
                        <<  "Second instance found: " << keyword);
                    if (keyword == "to_left")
                        leftRight = SRG_TO_LEFT;
                    else
                        leftRight = SRG_TO_RIGHT;
                }
                // check if the keyword is a spatial specifier
                else if (keyword == "close_by" || keyword == "far_away") {
                    // ASSERTION: The front/back region was not specified
                    PARSER_ASSERT((spatialSearch == KoduConditionSee::SRD_UNRESTRICTED),
                        errorMessage << "A close_by/far_away radius specifier was already specified.\n"
                        <<  "Second instance found: " << keyword);
                    if (keyword == "close_by")
                        spatialSearch = KoduConditionSee::SRD_CLOSE_BY;
                    else
                        spatialSearch = KoduConditionSee::SRD_FAR_AWAY;
                }
                // the user specified the wrong keyword
                else {
                    // ASSERTION: The user entered a wrong keyword
                    PARSER_ASSERT((ERROR),
                        errorMessage << "The keyword \"" << keyword << "\" cannot be used with the "
                        << "See condition.");
                }
                // bookkeeping
                GeneralFncs::destroyPtrInVector(mods, 0);
            }
            // the user something other than a keyword and is therefore illegal
            else {
                PARSER_ASSERT((ERROR),
                    errorMessage << "Token " << tokenCount << " cannot be used with the See condition.");
            }
            // increment the token counter
            tokenCount++;
        }
        // ASSERTION: the user specified a object type
        PARSER_ASSERT((!objectType.empty()),
            errorMessage << "An object type must be specified (e.g. tree, apple, or rock).");

        // ASSERTION: the user specified a object color
        PARSER_ASSERT((!objectColor.empty()),
            errorMessage << "An object color must be specified (e.g. red, green, blue).");

        // create the condition
        return (new KoduConditionSee(objectType, objectColor, (frontBack | leftRight), spatialSearch));
    }
    
    KoduConditionScored* Parser::KodeCreator::createScoredKode(std::vector<TokenBase*>& mods) {
        // optional modifiers
        NumericGenerator tempNumReq(0, 0);
        bool notEnabled = false;
        KoduConditionScored::CompareType_t comparisonType;
        bool comparisonOperatorSet = false;
        std::string scoreDesignator;

        // checkers
        int tokenCount = 1;
        bool handledNumerics = false;

        // parsing loop
        while (!mods.empty()) {
            // check if the token is an inequality operator
            if (isInequalityOperator(mods[0])) {
                // ASSERTION: A number was not specified before an inequality sign
                PARSER_ASSERT((handledNumerics == false),
                    errorMessage << "A numeric value cannot be specified before an "
                    << "inequality operator.");
                // ASSERTION: The comparison type was not set
                PARSER_ASSERT((comparisonOperatorSet == false),
                    errorMessage << "The comparison type was already set.");
                std::string keyword = mods[0]->getKeywordData();
                // get the comparison type
                if (keyword == "equals")
                    comparisonType = KoduConditionScored::CT_EQUALS;
                else if (keyword == "not_equals")
                    comparisonType = KoduConditionScored::CT_NOT_EQUALS;
                else if (keyword == ">=")
                    comparisonType = KoduConditionScored::CT_GT_EQUAL;
                else if (keyword == "<=")
                    comparisonType = KoduConditionScored::CT_LT_EQUAL;
                else if (keyword == "above")
                    comparisonType = KoduConditionScored::CT_ABOVE;
                else
                    comparisonType = KoduConditionScored::CT_BELOW;
                comparisonOperatorSet = true;
                // bookkeeping
                GeneralFncs::destroyPtrInVector(mods, 0);
            }
            // check if the token is numeric
            else if (isNumericSpecifier(mods[0])) {
                // ASSERTION: The numeric/random number parser has not been called for this "phrase"
                PARSER_ASSERT((handledNumerics == false),
                    errorMessage << "Token " << tokenCount << " is illegal. A numeric "
                    << "specifier (number and/or random modifiers) was already specified.");
                // get the numeric request
                // ASSERTION: The numeric/random number parser did not fail
                PARSER_ASSERT((numericGenParser(mods, tempNumReq) == true),
                    errorMessage << "An error occurred while parsing the numeric/random token(s).");
                // note that numbers have been handled
                handledNumerics = true;
                continue;
            }
            // check if it is a color or letter
            else if (isScoreDesignator(mods[0])) {
                std::string keyword = mods[0]->getKeywordData();
                // ASSERTION: the scoring letter was not already specified
                PARSER_ASSERT((scoreDesignator.empty()),
                    errorMessage << "The scored condition cannot have more than one designator. "
                    << "Second designator: " << keyword << ".");
                // assign the designator
                scoreDesignator = keyword;
            }
            // check if the token is a keyword
            else if (mods[0]->isKeywordToken()) {
                // get the keyword
                std::string keyword = mods[0]->getKeywordData();
                // check if the keyword is "not"
                if (keyword == "not") {
                    // ASSERTION: This is the first occurrence of the "not" modifier
                    PARSER_ASSERT((notEnabled == false),
                        errorMessage << "The \"not\" modifier can only be used once.");
                    notEnabled = true;
                }
                // the keyword is illegal
                else {
                    // ASSERTION: This keyword token is illegal
                    PARSER_ASSERT((ERROR),
                        errorMessage << "The keyword \"" << keyword << "\" cannot be used "
                        << "with the Scored condition.");
                }
                // bookkeeping
                GeneralFncs::destroyPtrInVector(mods, 0);
            }
            // the token is illegal
            else {
                // ASSERTION: The user added an illegal token
                PARSER_ASSERT((ERROR),
                    errorMessage << "Token " << tokenCount << " cannot be used with the Scored condition.");
            }
            tokenCount++;
        }
        // if a designator was not assigned, assign it the default color
        if (scoreDesignator.empty())
            scoreDesignator = Parser::koduDefaultDesignator;
        // return the condition
        return (new KoduConditionScored(notEnabled, comparisonType, tempNumReq, scoreDesignator, handledNumerics));
    }

    KoduConditionTimer* Parser::KodeCreator::createTimerKode(std::vector<TokenBase*>& mods) {
        // ASSERTION: There are 0 - 6 modifiers
        PARSER_ASSERT((0 <= mods.size() && mods.size() <= 6),
            errorMessage << "The Timer condition should have 0 - 6 modifiers.");

        // mandatory modifiers
        NumericGenerator tempNumReq(0.25, 0);

        // optional modifiers
        bool notEnabled = false;
        
        // checkers
        int tokenCount = 1;
        bool handledNumerics = false;

        // parsing loop
        while (!mods.empty()) {
            // check if the token is numeric
            if (isNumericSpecifier(mods[0])) {
                // ASSERTION: The numeric/random number parser has not been called for this "phrase"
                PARSER_ASSERT((handledNumerics == false),
                    errorMessage << "Numeric specifiers for this action were already handled. "
                    << "Token " << tokenCount << " is illegal.");
                // get the numeric request
                // ASSERTION: The numeric/random number parser did not fail
                PARSER_ASSERT((numericGenParser(mods, tempNumReq) == true),
                    errorMessage << "An error occurred while parsing the numeric specifier token(s) "
                    << "See above.");
                // note that numbers have been handled
                handledNumerics = true;
            }
            // check if the token is a keyword
            else if (mods[0]->isKeywordToken()) {
                // get the keyword
                std::string keyword = mods[0]->getKeywordData();
                // check if the keyword is "not"
                if (keyword == "not") {
                    // ASSERTION: This is the first occurrence of the "not" modifier
                    PARSER_ASSERT((notEnabled == false),
                        errorMessage << "The \"not\" modifier can only be used once.");
                    notEnabled = true;
                }
                // something is WRONG!
                else {
                    // ASSERTION: This keyword token is illegal
                    PARSER_ASSERT((ERROR),
                        errorMessage << "The keyword \"" << keyword << "\" cannot be used "
                        << "with the Timer condition.");
                }
                // bookkeeping
                GeneralFncs::destroyPtrInVector(mods, 0);
            }
            // the token is illegal
            else {
                // ASSERTION: The user added an illegal token
                PARSER_ASSERT((ERROR),
                    errorMessage << "Token " << tokenCount << " cannot be used with the Timer condition.");
            }
            tokenCount++;
        }
        // create the condition
        return (new KoduConditionTimer(notEnabled, tempNumReq));
    }
}