#include "Kodu/Objects/KoduObject.h"

namespace Kodu {

    const std::string& KoduObject::getColor() const {
        return objectColor;
    }

    const std::string& KoduObject::getType() const {
        return objectType;
    }

    bool KoduObject::isLandmark() const {
        return objectIsLandmark;
    }

    bool KoduObject::isMovable() const {
        // landmark objects should not be movable
        return (!isLandmark());
    }

    bool KoduObject::foundValidMatch() const {
        return matchingObject.isValid();
    }

    const DualCoding::ShapeRoot& KoduObject::getMatchingObject() const {
        return matchingObject;
    }

    void KoduObject::setMatchingObject(const DualCoding::ShapeRoot& kShape) {
        matchingObject = kShape;
    }
}
