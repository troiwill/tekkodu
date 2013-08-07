#include "KoduInequalityOperator.h"

namespace Kodu {
	std::set<std::string> KoduInequalityOperator::inequalityOperators;
	
	const std::set<std::string>& KoduInequalityOperator::getInequalitySet() {
		return inequalityOperators;
	}

	void KoduInequalityOperator::initialize() {
		inequalityOperators.insert("above");
		inequalityOperators.insert("below");
		inequalityOperators.insert("equals");
		inequalityOperators.insert("not_equals");
		inequalityOperators.insert(">=");
		inequalityOperators.insert("<=");
	}

	bool KoduInequalityOperator::isValidInequality(const std::string& inequality) {
		return (bool)inequalityOperators.count(inequality);
	}
}