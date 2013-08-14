// Tekkodu Library
#include "PerceptionSearch.h"

// Tekkotsu Library
#include "DualCoding/VRmixin.h"
#include "DualCoding/ShapeTypes.h"

namespace Kodu {
	
	bool IsLeftOfMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			return shapePoint.coordY() > thisShapeLocation.coordY();
		}
		return false;*/
		//return (kShape.getData().getCentroid().isLeftOf(thisShapeLocation));
		float bearing2ThisShape = (kShape.getData().getCentroid() - thisAgent.getData().getCentroid()).atanYX();
		AngSignPi dtheta = bearing2ThisShape - DualCoding::VRmixin::theAgent.getData().getOrientation();
		return (dtheta > 0.0f ? true : false);
	}

	bool IsRightOfMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			return shapePoint.coordY() < thisShapeLocation.coordY();
		}*/
		return false;
		//return (kShape.getData().getCentroid().isRightOf(thisShapeLocation));
	}

	bool IsInFrontMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			return shapePoint.coordX() > thisShapeLocation.coordX();
		}*/
		return false;
		//return (kShape.getData().getCentroid().isAbove(thisShapeLocation));
	}

	bool IsBehindMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			return shapePoint.coordX() < thisShapeLocation.coordX();
		}*/
		return false;
		//return (kShape.getData().getCentroid().isBelow(thisShapeLocation));
	}

	bool IsCloseByMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			float dx = shapePoint.coordX() - thisShapeLocation.coordX();
			float dy = shapePoint.coordY() - thisShapeLocation.coordY();
			return (sqrt((dx * dx) + (dy * dy)) < 700.0f);
		}*/
		return false;
		//return ((kShape.getData().getCentroid().xyDistanceFrom(thisShapeLocation)) < 700);
	}

	bool IsFarAwayFromMe::operator()(const DualCoding::ShapeRoot& kShape) const {
		/*const DualCoding::Point& shapePoint = kShape.getData().getCentroid();
		if (shapePoint.refFrameType == DualCoding::allocentric
			&& thisShapeLocation.refFrameType == DualCoding::allocentric)
		{
			float dx = shapePoint.coordX() - thisShapeLocation.coordX();
			float dy = shapePoint.coordY() - thisShapeLocation.coordY();
			return (sqrt((dx * dx) + (dy * dy)) > 1000.0f);
		}*/
		return false;
		//return ((kShape.getData().getCentroid().xyDistanceFrom(thisShapeLocation)) > 1000);
	}
}