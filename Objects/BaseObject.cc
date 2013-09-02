#include "Kodu/Objects/BaseObject.h"

namespace Kodu {

    template<class ShapeType>
    const std::string& BaseObject<ShapeType>::getColor() const {
        return objectColor;
    }

    template<class ShapeType>
    const std::string& BaseObject<ShapeType>::getType() const {
        return objectType;
    }

    template<class ShapeType>
    bool BaseObject<ShapeType>::isLandmark() const {
        return objectIsLandmark;
    }

    template<class ShapeType>
    bool BaseObject<ShapeType>::isMovable() const {
        // landmark objects should not be movable
        return (!isLandmark());
    }

    template<class ShapeType>
    bool BaseObject<ShapeType>::foundValidMatch() const {
        return matchingObject.isValid();
    }

    template<class ShapeType>
    const ShapeType& BaseObject<ShapeType>::getMatchingObject() const {
        return matchingObject;
    }

    template<class ShapeType>
    void BaseObject<ShapeType>::setMatchingObject(const ShapeType& kShape) {
        matchingObject = kShape;
    }
}
