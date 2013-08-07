#include "ColorTypes.h"

namespace Kodu {
	std::set<std::string> ColorTypes::colorNames;
	std::string ColorTypes::defaultColor;
	
	const std::set<std::string>& ColorTypes::getColorSet() {
		return colorNames;
	}

	void ColorTypes::initialize() {
		defaultColor = "red";
		colorNames.insert(defaultColor);
		colorNames.insert("green");
		colorNames.insert("blue");
	}

	bool ColorTypes::isValidColor(const std::string& colorName) {
		return static_cast<bool>(colorNames.count(colorName));
	}

	const std::string& ColorTypes::getDefaultColor() {
		return defaultColor;
	}
}