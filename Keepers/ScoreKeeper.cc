#include "Kodu/Keepers/ScoreKeeper.h"

namespace Kodu {
	std::map<std::string,int> ScoreKeeper::scoreBoard;

	int ScoreKeeper::addScore(const std::string& scoreKey, int value) {
		if (!scoreExists(scoreKey)) {
			registerScore(scoreKey);
		}
		scoreBoard[scoreKey] += value;
		// TODO (20/JUL/13) how to report a value in stdout with color using the KoduColor class
		std::cout << "ScoreKeeper: Score {" << scoreKey << "} = "
				  << scoreBoard[scoreKey] << std::endl;
		return scoreBoard[scoreKey];
	}
	
	int ScoreKeeper::checkScoreValue(const std::string& scoreKey) {
		if (!scoreExists(scoreKey)) {
			registerScore(scoreKey);
		}
		return scoreBoard[scoreKey];
	}
	
	void ScoreKeeper::clearScores() {
		std::cout << "ScoreKeeper: Clearing score board.\n";
		ScoreKeeper::initialize();
	}
	
	void ScoreKeeper::initialize() {
		scoreBoard.clear();
	}

	bool ScoreKeeper::registerScore(const std::string& scoreKey) {
		if (!scoreExists(scoreKey)) {
			scoreBoard.insert(std::pair<std::string,int>(scoreKey, 0));
			std::cout << "ScoreKeeper: Registering score with key {"
					  << scoreKey << "}.\n";
		}
		return scoreExists(scoreKey);
	}
	
	bool ScoreKeeper::scoreExists(const std::string& scoreKey) {
		return (scoreBoard.count(scoreKey) > 0);
	}

	int ScoreKeeper::setScore(const std::string& scoreKey, int value) {
		if (!scoreExists(scoreKey)) {
			registerScore(scoreKey);
		}
		scoreBoard[scoreKey] = value;
		// TODO (20/JUL/13) how to report a value in stdout with color using the KoduColor class
		std::cout << "ScoreKeeper: Score {" << scoreKey << "} = "
				  << scoreBoard[scoreKey] << std::endl;
		return scoreBoard[scoreKey];
	}

	int ScoreKeeper::subtractScore(const std::string& scoreKey, int value) {
		if (!scoreExists(scoreKey)) {
			registerScore(scoreKey);
		}
		scoreBoard[scoreKey] -= value;
		// TODO (20/JUL/13) how to report a value in stdout with color using the KoduColor class
		std::cout << "ScoreKeeper: Score {" << scoreKey << "} = "
				  << scoreBoard[scoreKey] << std::endl;
		return scoreBoard[scoreKey];
	}
}