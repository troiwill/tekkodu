// Kodu Library
#include "Kodu/Behaviors/KoduConditionSee.h"

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/VRmixin.h"

// Standard C++ Library
#include <algorithm>

namespace Kodu {

	bool KoduConditionSee::evaluate() {
		// Tekkotsu. Where the robot (presumably) is in the world.
		DualCoding::Shape<DualCoding::AgentData> agent = DualCoding::VRmixin::theAgent;
		DualCoding::Point agentLocation = agent.getData().getCentroid();
		
		// Tekkotsu function. Returns all the objects that are Cylinders
		NEW_SHAPEVEC(objects, DualCoding::CylinderData,
						DualCoding::select_type<DualCoding::CylinderData>(DualCoding::VRmixin::worldShS));
		std::cout << "Size of objects vec: " << objects.size() << std::endl;

		// Tekkotsu function. Returns all objects with a specified color
		objects = DualCoding::subset(objects, DualCoding::IsColor(objColor));
		
		// test if the search region is unrestricted
		if (searchRegion != SRG_UNRESTRICTED) {
			// test if the search area should be limited to the left or right sides
			if (searchRegion & SRG_TO_LEFT) {
				std::cout << "Checking map for objects to the left of me\n";
				//objects = DualCoding::subset(objects, DualCoding::IsLeftOfThis(agent));
				objects = DualCoding::subset(objects, IsLeftOfMe(agent));
			} else if (searchRegion & SRG_TO_RIGHT) {
				std::cout << "Checking map for objects to the right of me\n";
				//objects = DualCoding::subset(objects, DualCoding::IsRightOfThis(agent));
				objects = DualCoding::subset(objects, IsRightOfMe(agent));
			}

			// test if the search area should be limited to the front or back
			if (searchRegion & SRG_IN_FRONT) {
				std::cout << "Checking map for objects in front me\n";
				//objects = DualCoding::subset(objects, DualCoding::IsAboveThis(agent));
				objects = DualCoding::subset(objects, IsInFrontMe(agent));
			} else if (searchRegion & SRG_BEHIND) {
				std::cout << "Checking map for objects behind me\n";
				//objects = DualCoding::subset(objects, DualCoding::IsBelowThis(agent));
				objects = DualCoding::subset(objects, IsBehindMe(agent));
			}
		}

		// test if the search radius is unrestricted
		if (searchRadius != SRD_UNRESTRICTED) {
			if (searchRadius == SRD_CLOSE_BY) {
				std::cout << "Checking map for objects close by me\n";
				//objects = DualCoding::subset(objects, IsCloseByAgent(agent));
				objects = DualCoding::subset(objects, IsCloseByMe(agent));
			} else {
				std::cout << "Checking map for objects far away from me\n";
				//objects = DualCoding::subset(objects, IsFarAwayFromAgent(agent));
				objects = DualCoding::subset(objects, IsFarAwayFromMe(agent));
			}
		}

		/*
		if ((searchRegion != SRG_UNRESTRICTED) || (searchRadius != SRD_UNRESTRICTED)) {
			// Tekkotsu macro for iterating over a vector of Shapes
			SHAPEVEC_ITERATE(objects, DualCoding::CylinderData, currObject)
				DualCoding::Point currObjLocation = currObject.getData().getCentroid();
				if (searchRegion != SRG_UNRESTRICTED) {
					// test if the search area should be limited to the right or left sides
					if ((searchRegion & SRG_TO_LEFT) && !currObjLocation.isLeftOf(agentLocation)) {
						std::remove_copy(objects.begin(), objects.end(), currObject);
						continue;
					} else if ((searchRegion & SRG_TO_RIGHT) && !currObjLocation.isRightOf(agentLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
						continue;
					}

					// test if the search area should be limited to the front or back
					if ((searchRegion & SRG_BEHIND) && !currObjLocation.isBelow(agentLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
						continue;
					} else if ((searchRegion & SRG_IN_FRONT) && !currObjLocation.isAbove(agentLocation)) {
						std::remove(objects.begin(), objects.end(), currObject);
						continue;
					}
				}

				// test if the search area should be "close by" or "far away" (units are in millimeters)
				if (searchRadius != SRD_UNRESTRICTED) {
					float distance = agentLocation.xyDistanceFrom(currObjLocation);
					if (searchRadius == SRD_CLOSE_BY && distance > 700) {
						std::remove(objects.begin(), objects.end(), currObject);
						continue;
					} else if (searchRadius == SRD_FAR_AWAY && distance < 1000) {
						std::remove(objects.begin(), objects.end(), currObject);
						continue;
					}
				}
			END_ITERATE;
		}
		*/

		// units = millimeters.
		float smallestDistance = 200000;
		// Tekkotsu macro for iterating over a vector of Shapes
		SHAPEVEC_ITERATE(objects, DualCoding::CylinderData, currObject)
			float currDistance = agentLocation.xyDistanceFrom(currObject.getData().getCentroid());
			if (currDistance < smallestDistance) {
				smallestDistance = currDistance;
				refdObject = currObject;
			}
		END_ITERATE;
		
		// Reports if there is at least one valid object after all tests have been performed (above)
		std::cout << "Refd object is" << (refdObject.isValid() ? " " : " not ") << "valid.\n";
		// If there is one valid remaining, the robot will react to that object
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
		// TODO (9/Aug/2013)
		// do I need to clear the Shape object?
	}
	
	void KoduConditionSee::printAttrs() {

	}
}