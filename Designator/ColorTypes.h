#ifndef KODU_COLOR_H_
#define KODU_COLOR_H_

// C++ Library
#include <set>
#include <string>

// Kodu Library
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {
	
	class ColorTypes {
	public:
		//! Returns the container with all the color names
		static const std::set<std::string>& getColorSet();

		//! Returns the default color
		static const std::string& getDefaultColor();

		//! Initializes the color container
		static void initialize();

		//! Checks if the color is valid and usable
		static bool isValidColor(const std::string&);

	private:
		//! Contains the container of all valid colors
		static std::set<std::string> colorNames;
		static std::string defaultColor;
		
		DISALLOW_INSTANTIATION(ColorTypes);
	};
}

#endif // KODU_COLOR_H_