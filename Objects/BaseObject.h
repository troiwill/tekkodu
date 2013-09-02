#ifndef BASE_OBJECTS_H_
#define BASE_OBJECTS_H_

// Tekkodu Library
#include "Kodu/Primitives/PerceptionSearch.h"

// Tekkotsu Library
#include "DualCoding/ShapeRoot.h"

// C++ Library
#include <string>

namespace Kodu {
    //! Base Object template class
    template<class ShapeType>
    class BaseObject {
    public:
        //! Constructor
        BaseObject<ShapeType>(const std::string& kObjectColor, const std::string& kObjectType,
            SearchLocation_t locationsToSearch, bool canBeLandmark)
          : objectColor(kObjectColor),
            objectType(kObjectType),
            searchLocations(locationsToSearch),
            objectIsLandmark(canBeLandmark),
            matchingObject()
        { }

        //! Copy constructor
        BaseObject<ShapeType>(const BaseObject<ShapeType>& kObject)
          : objectColor(kObject.objectColor),
            objectType(kObject.objectType),
            searchLocations(kObject.searchLocations),
            objectIsLandmark(kObject.objectIsLandmark),
            matchingObject(kObject.matchingObject)
        { }

        //! Destructor
        virtual ~BaseObject<ShapeType>() {
            // no explicit implementation
        }

        //! Assignment operator
        BaseObject<ShapeType>& operator=(const BaseObject<ShapeType>& kObject) {
            if (this != &kObject) {
                objectColor = kObject.objectColor;
                objectType = kObject.objectType;
                searchLocations = kObject.searchLocations;
                objectIsLandmark = kObject.objectIsLandmark;
                matchingObject = kObject.matchingObject;
            }
            return *this;
        }

        //! Returns the object's color
        const std::string& getColor() const;

        //! Returns the object's type
        const std::string& getType() const;

        //! Returns if an object is a landmark or not
        bool isLandmark() const;

        //! Returns whether or not an object is movable
        bool isMovable() const;

        //! 
        bool foundValidMatch() const;

        //! Returns the object that matches the description
        virtual const ShapeType& getMatchingObject() const;

        //! Sets the
        virtual void setMatchingObject(const ShapeType&);

        //! Returns whether the parameter is the same type as the calling class
        static bool isSameTypeAs(const BaseObject<ShapeType>*);

    protected:
        std::string objectColor;            //!< the object's color
        std::string objectType;             //!< the object's type (e.g. tree, rock, apple)
        SearchLocation_t searchLocations;   //!< the locations relative to the perceiver to search for an object
        bool objectIsLandmark;              //!< can "this" object be used as a landmark
        ShapeType matchingObject;           //!< the object that matches search criteria (color, type, etc)
    };
}

#endif // end of BASE_OBJECTS_H_
