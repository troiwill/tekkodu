#include "Kodu/Keepers/ObjectKeeper.h"

namespace Kodu {
	DualCoding::Shape<DualCoding::CylinderData> ObjectKeeper::tempObject;
	bool ObjectKeeper::isValid = false;
	DualCoding::Shape<DualCoding::CylinderData> ObjectKeeper::invalidObject;
}