#ifndef KODU_PAGE_H_
#define KODU_PAGE_H_

// C++ Library
#include <string>
#include <vector>

// Kodu Library
#include "Kodu/KoduRule.h"

#include "Kodu/General/GeneralFncs.h"

namespace Kodu {
	//! Kodu Page
	class KoduPage {
	private:
		unsigned int pageNumber;				//!< Page number
		std::vector<KoduRule*> rules;			//!< Collection of rules for a page
		std::vector<std::string> objectColors;
		
	public:
		//! Constructor
		explicit KoduPage(unsigned int kPageNumber)
		  : pageNumber(kPageNumber),
			rules(),
			objectColors()
		{ }
		
		//! Destructor
		~KoduPage() {
			GeneralFncs::destroyAllPtrsInVector(rules);
		}
		
		//! Adds a Kodu rule to the end of the collection.
		void addKoduRule(KoduRule*);
		
		//! Get the page number
		unsigned int getPageNumber() const;
		
		//! Returns a rule with a specified ID (number)
		KoduRule* getRule(unsigned int number);

		//! Returns a rule in a specified position of the vector
		KoduRule* getRuleInPos(unsigned int pos);
		
		//! Returns the size of the rules vector
		unsigned int getRuleCount() const;

		const std::vector<std::string> getObjectDescriptors() const;
		void addObjectDescriptor(const std::string& descriptor);
		
	private:
		//! Disallow the copy constructor and assignment operator
		DISALLOW_COPY_ASSIGN(KoduPage);
	};
}
#endif // KODU_PAGE_H_