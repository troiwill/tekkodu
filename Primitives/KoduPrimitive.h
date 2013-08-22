#ifndef KODU_PRIMITIVE_H_
#define KODU_PRIMITIVE_H_

// C++ Library
#include <iostream>
#include <string>

namespace Kodu {
	
	//! Kodu Primitive
	class KoduPrimitive {	
	public:
		//! Constructor
		KoduPrimitive(const std::string& kPrimitiveType)
		  : primitiveType(kPrimitiveType)
		{ }
		
		//! Copy constructor
		KoduPrimitive(const KoduPrimitive& kBehavior)
		  : primitiveType(kBehavior.primitiveType)
		{ }

		//! Destructor
		virtual ~KoduPrimitive() {
			// no explicit implementation
		}
		
		//! Assignment operator
		KoduPrimitive& operator=(const KoduPrimitive& kBehavior) {
			if (this != &kBehavior) {
				primitiveType = kBehavior.primitiveType;
			}
			return *this;
		}

		//! Returns the name of the derived primitive
		const std::string& getPrimitiveType() const {
			return primitiveType;
		}
		
		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize() = 0;

		//! Prints the attributes for a particular primitive (pure virtual function)
		virtual void printAttrs() {
			std::cout << "Primitive Type: " << primitiveType << std::endl;
		}

	protected:
		std::string primitiveType;	//!< The name of the derived primitive
	};
} // end of Kodu namespace

#endif // KODU_PRIMITIVE_H_
