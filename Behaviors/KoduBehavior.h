#ifndef KODU_BEHAVIOR_H_
#define KODU_BEHAVIOR_H_

// C++ Library
#include <iostream>
#include <string>

namespace Kodu {
	
	//! Kodu Behavior
	class KoduBehavior {	
	public:
		//! Constructor
		KoduBehavior(const std::string& kBehaviorType)
		  : behaviorType(kBehaviorType)
		{ }
		
		//! Copy constructor
		KoduBehavior(const KoduBehavior& kBehavior)
		  : behaviorType(kBehavior.behaviorType)
		{ }

		//! Destructor
		virtual ~KoduBehavior() {
			// no explicit implementation
		}
		
		//! Assignment operator
		KoduBehavior& operator=(const KoduBehavior& kBehavior) {
			if (this != &kBehavior) {
				behaviorType = kBehavior.behaviorType;
			}
			return *this;
		}

		//! Returns the name of the derived behavior
		const std::string& getBehaviorType() const {
			return behaviorType;
		}
		
		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize() = 0;

		//! Prints the attributes for a particular behavior (pure virtual function)
		virtual void printAttrs() {
			std::cout << "Behavior Type: " << behaviorType << std::endl;
		}

	protected:
		std::string behaviorType;	//!< The name of the derived behavior
	};
} // end of Kodu namespace

#endif // KODU_BEHAVIOR_H_
