#include "Kodu/Behaviors/KoduActionPlay.h"

namespace Kodu {
	/*
	const PlayRequest& KoduActionPlay::execute() {
		if (onceEnabled)
			actionDisabled = true;	// prevents rule from being evaluated/executed
		// ==== for debugging ==== //
		std::cout << getBehaviorType() << ": Playing sound \"" << playreq.soundFile << "\"\n";
		// ======================= //
		return playreq;
	}
	*/

	const std::string& KoduActionPlay::getSoundFile() {
		if (onceEnabled)
			actionHasAlreadyRan = true;
		return soundFile.getLiteralString();
	}

	void KoduActionPlay::reinitialize() {
		KoduAction::reinitialize();
	}
	
	void KoduActionPlay::printAttrs() {
		KoduAction::printAttrs();
		soundFile.printAttrs();
		PRINT_ATTRS("Once modifier", onceEnabled);
	}
}