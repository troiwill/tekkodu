#ifndef OBJECT_KEEPER_H_
#define OBJECT_KEEPER_H_

#include "DualCoding/DualCoding.h"
#include "DualCoding/ShapeCylinder.h"

namespace Kodu {

	// temporary fix (entire class will be removed later and a better solution implemented)
	class ObjectKeeper {
	public:
		static DualCoding::Shape<DualCoding::CylinderData> tempObject;
		static bool isValid;
		static DualCoding::Shape<DualCoding::CylinderData> invalidObject;
	};
}

#endif // OBJECT_KEEPER_H_