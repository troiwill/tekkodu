#include "Kodu/Behaviors/KoduConditionSee.h"
#include "DualCoding/DualCoding.h"
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/BaseData.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/VRmixin.h"
#include <algorithm>

namespace Kodu {

	bool KoduConditionSee::evaluate() {
		//bool rv = false;
		DualCoding::Point agentLocation = DualCoding::VRmixin::theAgent.getData().getCentroid();
		// get all the objects of interest (assuming all objects are cylinders)
		NEW_SHAPEVEC(objects, DualCoding::CylinderData,
						DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));
		std::cout << "Size of objects vec: " << objects.size() << std::endl;

		// ASSUMPTION: the user has to specify a color
		objects = subset(objects, DualCoding::IsColor(objColor));
		//refdObject = find_if(objects, DualCoding::IsColor(objColor));

		if ((searchRegion != SRG_UNRESTRICTED) || (searchRadius != SRD_UNRESTRICTED)) {
			
			SHAPEVEC_ITERATE(objects, DualCoding::CylinderData, currObject)
				DualCoding::Point currObjLocation = currObject.getData().getCentroid();
				if (searchRegion != SRG_UNRESTRICTED) {
					// test if the search area should be limited to the right or left sides
					if ((searchRegion & SRG_TO_LEFT) && !agentLocation.isRightOf(currObjLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
					} else if ((searchRegion & SRG_TO_RIGHT) && !agentLocation.isLeftOf(currObjLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
					}

					// test if the search area should be limited to the front or back
					if ((searchRegion & SRG_BEHIND) && !agentLocation.isAbove(currObjLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
					} else if ((searchRegion & SRG_IN_FRONT) && !agentLocation.isBelow(currObjLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
					}
				}

				// test if the search area should be "close by" or "far away"
				if (searchRadius != SRD_UNRESTRICTED) {
					float distance = agentLocation.xyDistanceFrom(currObjLocation);
					if (searchRadius == SRD_CLOSE_BY && distance > 850) {
						std::remove(objects.begin(), objects.end(), currObject);
					} else if (searchRadius == SRD_FAR_AWAY && distance < 1250) {
						std::remove(objects.begin(), objects.end(), currObject);
					}
				}
			END_ITERATE;
		}

		float smallestDistance = 200000;
		SHAPEVEC_ITERATE(objects, DualCoding::CylinderData, currObject)
			float currDistance = agentLocation.xyDistanceFrom(currObject.getData().getCentroid());
			if (currDistance < smallestDistance) {
				smallestDistance = currDistance;
				refdObject = currObject;
			}
		END_ITERATE;
		
		std::cout << "Refd object is " << (refdObject.isValid() ? " " : "not ") << "valid.\n";
		std::cout << "Object type and color: " << objType << ", " << objColor << std::endl;
		if (refdObject.isValid()) {
			ObjectKeeper::tempObject = refdObject;
			ObjectKeeper::isValid = true;
			return true;
		}
		return false;
	}

	const std::string& KoduConditionSee::getObjectColor() const {
		return objColor;
	}

	const std::string& KoduConditionSee::getObjectType() const {
		return objType;
	}

	const DualCoding::Shape<DualCoding::CylinderData>& KoduConditionSee::getTargetObject() {
		return refdObject;
	}

	void KoduConditionSee::reinitialize() {
		// do I need to clear the Shape object?
	}
	
	void KoduConditionSee::printAttrs() {

	}
}