#ifndef KODU_CONDITION_SEE_H_
#define KODU_CONDITION_SEE_H_

// Tekkodu Library
#include "Kodu/Behaviors/KoduCondition.h"
#include "Kodu/Behaviors/PerceptionSearch.h"
#include "Kodu/Keepers/ObjectKeeper.h"

// Tekkotsu Library
#include "DualCoding/CylinderData.h"
#include "DualCoding/ShapeCylinder.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"

// C++ Library
#include <iostream>

namespace Kodu {

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