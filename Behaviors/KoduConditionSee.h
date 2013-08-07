#ifndef KODU_CONDITION_SEE_H_
#define KODU_CONDITION_SEE_H_

// C++ Library
#include <iostream>

// Kodu Library
#include "Kodu/Behaviors/KoduCondition.h"
#include "Kodu/Keepers/ObjectKeeper.h"

// Tekkotsu Library
#include "DualCoding/DualCoding.h"
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/BaseData.h"

namespace Kodu {

	enum SearchRegion_t {
		SRG_UNRESTRICTED	= 0,
		SRG_BEHIND			= 1L << 1,
		SRG_IN_FRONT		= 1L << 2,
		SRG_TO_LEFT			= 1L << 3,
		SRG_TO_RIGHT		= 1L << 4
	};
	
	inline
	SearchRegion_t operator|(SearchRegion_t rs1, SearchRegion_t rs2) {
		return SearchRegion_t(static_cast<int>(rs1) | static_cast<int>(rs2));
	}

	inline
	SearchRegion_t operator|=(SearchRegion_t rs1, SearchRegion_t rs2) {
		return (rs1 = rs1 | rs2);
	}

	inline
	SearchRegion_t operator&(SearchRegion_t rs1, SearchRegion_t rs2) {
		return SearchRegion_t(static_cast<int>(rs1) & static_cast<int>(rs2));
	}

	class KoduConditionSee : public KoduCondition {
	public:
		enum SearchRadius_t {
			SRD_UNRESTRICTED	= 1L << 0,
			SRD_CLOSE_BY		= 1L << 1,
			SRD_FAR_AWAY		= 1L << 2
		};

		//! Constructor
		KoduConditionSee(const std::string& kObjectType, const std::string& kObjectColor,
			SearchRegion_t regionToSearch, SearchRadius_t radiusToSearch)
		  : KoduCondition("KoduConditionSee"),
		  	objType(kObjectType),
		  	objColor(kObjectColor),
		  	searchRegion(regionToSearch),
		  	searchRadius(radiusToSearch),
		  	refdObject()
		{ }

		//! Copy constructor
		KoduConditionSee(const KoduConditionSee& kCondition)
		  : KoduCondition(kCondition),
		  	objType(kCondition.objType),
		  	objColor(kCondition.objColor),
		  	searchRegion(kCondition.searchRegion),
		  	searchRadius(kCondition.searchRadius),
		  	refdObject(kCondition.refdObject)
		{ }

		//! Destructor
		~KoduConditionSee() {
			// no explicit implementation
		}

		//! Assignment operator
		KoduConditionSee& operator=(const KoduConditionSee& kCondition) {
			if (this != &kCondition) {
				KoduCondition::operator=(kCondition);
				objType = kCondition.objType;
				objColor = kCondition.objColor;
				searchRegion = kCondition.searchRegion;
				searchRadius = kCondition.searchRadius;
				refdObject = kCondition.refdObject;
			}
			return *this;
		}

		//! Checks if the target object was seen
		virtual bool evaluate();

		//! Returns the target object's color
		const std::string& getObjectColor() const;

		//! Returns the target object's type
		const std::string& getObjectType() const;

		//! Returns the target object... if none is available, return an invalid object
		const DualCoding::Shape<DualCoding::CylinderData>& getTargetObject();

		//! Used to reinitialize certain variables (e.g. when switching to another page)
		virtual void reinitialize();
		
		//! Prints the attributes of a particular instance
		virtual void printAttrs();

	private:
		std::string objType;
		std::string objColor;
		SearchRegion_t searchRegion;
		SearchRadius_t searchRadius;
		DualCoding::Shape<DualCoding::CylinderData> refdObject;
	};
}

#endif // KODU_CONDITION_SEE_H_