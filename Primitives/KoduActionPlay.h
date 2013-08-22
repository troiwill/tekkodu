#ifndef KODU_ACTION_PLAY_H_
#define KODU_ACTION_PLAY_H_

// C++ Library
#include <iostream>
#include <string>

// Kodu Library
#include "Kodu/Primitives/KoduAction.h"
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Generators/KoduGenerators.h"

// TODO
// (25/01/13) find other modifiers for play action

namespace Kodu {

	//! Kodu Action Play class (derived from Kodu Action)
	class KoduActionPlay : public KoduAction {
	public:
		//! Constructor
		KoduActionPlay(const LiteralGenerator& kSoundFile, bool useOnce)
		  : KoduAction ("KoduActionPlay"),
			soundFile(kSoundFile),
			onceEnabled(useOnce)
		{ }
		
		//! Copy constructor
		KoduActionPlay(const KoduActionPlay& kAction)
		  : KoduAction(kAction),
		  	soundFile(kAction.soundFile),
		  	onceEnabled(kAction.onceEnabled)
		{ }

		//! Destructor
		~KoduActionPlay() {
			// no explicit implementation
		}
		
		//! Assignment operator
		KoduActionPlay& operator=(const KoduActionPlay& kAction) {
			if (this != &kAction) {
				KoduAction::operator=(kAction);
				soundFile = kAction.soundFile;
				onceEnabled = kAction.onceEnabled;
			}
			return *this;
		}

		//! Returns the name of the sound file
		const std::string& getSoundFile();
		
		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize();

		//! Prints the attributes of a particular instance
		virtual void printAttrs();

	private:
		LiteralGenerator soundFile;
		bool onceEnabled;
	};
}

#endif // KODU_ACTION_PLAY_H_
