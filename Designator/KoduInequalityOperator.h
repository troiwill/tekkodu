#ifndef KODU_INEQUALITY_OPERATOR_H_
#define KODU_INEQUALITY_OPERATOR_H_

// C++ Library
#include <set>
#include <string>

// Kodu Library
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {
	
	class KoduInequalityOperator {
	public:
		//! Returns the container with all the inequality names
		static const std::set<std::string>& getInequalitySet();

		//! Initializes the inequality container
		static void initialize();

		//! Checks if the inequality is valid and usable
		static bool isValidInequality(const std::string&);

	private:
		//! Contains the container of all valid inequality
		static std::set<std::string> inequalityOperators;
		
		DISALLOW_INSTANTIATION(KoduInequalityOperator);
	};
}

#endif // KODU_INEQUALITY_OPERATOR_H_