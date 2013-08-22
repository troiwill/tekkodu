#include "Kodu/Primitives/KoduActionPageSwitch.h"

namespace Kodu {
	
	int KoduActionPageSwitch::getPageNumber() {
		return (int)(pageNumb.getNumericValue());
	}

	void KoduActionPageSwitch::reinitialize() {
		KoduAction::reinitialize();
		// do nothing
	}

	void KoduActionPageSwitch::printAttrs() {
		KoduAction::printAttrs();
		std::cout << "Page requested: " << pageNumb.getNumericValue() << std::endl;
	}
}