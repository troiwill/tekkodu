#ifndef SCORE_KEEPER_H_
#define SCORE_KEEPER_H_

// C++ Library
#include <iostream>
#include <string>

// Kodu Library
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {
// ======================== KODU KEEPER TYPE: SCORE =================================== //
	//! Score Keeper
	class ScoreKeeper {
	public:
		//! Adds to a particular score
		static int addScore(const std::string&, int);
		
		//! Checks the value of a particular score
		static int checkScoreValue(const std::string&);
		
		//! Clears all the contents of the map (calls the initialize function)
		static void clearScores();
		
		//! Initializes the global score board (deletes the contents of the map)
		static void initialize();

		//! Registers an entry for a new score based on the key provided
		static bool registerScore(const std::string&);
		
		//! Checks if a particular score type (identified by color+letter key) exists
		static bool scoreExists(const std::string&);

		//! Sets a particular score
		static int setScore(const std::string&, int);
		
		//! Subtracts from a particular score
		static int subtractScore(const std::string&, int);

	private:
		//! Contains all the global scores
		static std::map<std::string,int> scoreBoard;

		//! Disallows users from creating an instance of this class
		DISALLOW_INSTANTIATION(ScoreKeeper);
	};
// ==================================================================================== //
} // end of Kodu namespace

#endif // SCORE_KEEPER_H_
