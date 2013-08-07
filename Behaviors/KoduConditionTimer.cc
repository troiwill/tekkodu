#include "Kodu/Behaviors/KoduConditionTimer.h"
#include "Shared/get_time.h"

namespace Kodu {
		
	bool KoduConditionTimer::evaluate() {
		// TODO (24/JUL/13) Is needToInitialize necessary?
		if (needToInitialize) {		// do initial setup for timer
			startTime = getTimeInSec();
			timerThreshold = numericGen.getNumericValue();
			needToInitialize = false;
		}
		currentTime = getTimeInSec();
		unsigned int timeDifference = currentTime - startTime;
		if (timeDifference >= timerThreshold) {
			timerThreshold = numericGen.getNumericValue();
			startTime = getTimeInSec();
			if (notEnabled) {
				return false;
				
			} else {
				// ==== for debugging ==== //
				std::cout << getBehaviorType() << ": Time reached [threshold = "
						  << timerThreshold << " secs]\n";
				// ======================= //
				return true;
			}
		} else if (timeDifference < timerThreshold && notEnabled) {
			// ==== for debugging ==== //
			std::cout << getBehaviorType() << ": Time not reached [threshold = "
					  << timerThreshold << " secs]\n";
			// ======================= //
			return true;
		}
		return false;
	}

	void KoduConditionTimer::reinitialize() {
		KoduCondition::reinitialize();
		needToInitialize = true;
	}
	
	void KoduConditionTimer::printAttrs() {
		KoduCondition::printAttrs();
		numericGen.printAttrs();
		PRINT_ATTRS("Not enabled", notEnabled);
		std::cout << "Current time difference: " << (currentTime - startTime) << std::endl;
		std::cout << "Current threshold: " << timerThreshold << std::endl;
	}

	unsigned int KoduConditionTimer::getTimeInSec() {
		return (get_time() / 1000);
	}
} // end of Kodu namespace