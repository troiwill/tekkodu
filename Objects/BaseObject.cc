#include "Kodu/Objects/BaseObject.h"

namespace Kodu {

    const std::string& BaseObject::getColor() const {
        return objectColor;
    }

    const std::string& BaseObject::getType() const {
        return objectType;
    }

    bool BaseObject::isLandmark() const {
        return objectIsLandmark;
    }

    bool BaseObject::isMovable() const {
        // landmark objects should not be movable
        return (!isLandmark());
    }

    bool BaseObject::foundValidMatch() const {
        return matchingObject.isValid();
    }

    const DualCoding::ShapeRoot& BaseObject::getMatchingObject() const {
        return matchingObject;
    }

    void BaseObject::setMatchingObject(const DualCoding::ShapeRoot& kShape) {
        matchingObject = kShape;
    }
}
