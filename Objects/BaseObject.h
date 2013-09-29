#ifndef BASE_OBJECTS_H_
#define BASE_OBJECTS_H_

// Tekkodu Library
#include "Kodu/Primitives/PerceptionSearch.h"

// Tekkotsu Library
#include "DualCoding/ShapeRoot.h"

// C++ Library
#include <string>

namespace Kodu {
    //! Base Object class
    class BaseObject {
    public:
        //! Constructor
        BaseObject(const std::string& kObjectColor, const std::string& kObjectType,
            SearchLocation_t locationsToSearch, bool canBeLandmark)
          : objectColor(kObjectColor),
            objectType(kObjectType),
            searchLocations(locationsToSearch),
            objectIsLandmark(canBeLandmark),
            matchingObject()
        { }

        //! Copy constructor
        BaseObject(const BaseObject& kObject)
          : objectColor(kObject.objectColor),
            objectType(kObject.objectType),
            searchLocations(kObject.searchLocations),
            objectIsLandmark(kObject.objectIsLandmark),
            matchingObject(kObject.matchingObject)
        { }

        //! Destructor
        virtual ~BaseObject() {
            // no explicit implementation
        }

        //! Assignment operator
        BaseObject& operator=(const BaseObject& kObject) {
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

        //! Returns true if an object is a landmark or not
        bool isLandmark() const;

        //! Returns true if an object is movable
        bool isMovable() const;

        //! Returns true if an object can move throughout the environment (autonomously)
        bool hasMobility() const;

        //! Returns true if the agent found an object that met the search criteria (color, type, etc)
        bool foundValidMatch() const;

        //! Returns the object that matches the description
        const DualCoding::ShapeRoot& getMatchingObject() const;

        //! Stores the object that matches the search criteria
        void setMatchingObject(const DualCoding::ShapeRoot&);

    protected:
        std::string objectColor;            //!< the object's color
        std::string objectType;             //!< the object's type (e.g. tree, rock, apple)
        SearchLocation_t searchLocations;   //!< the locations the perceiver should search for an object
        bool objectIsLandmark;              //!< can "this" object be used as a landmark
        DualCoding::ShapeRoot matchingObject; //!< the object that matches search criteria
    };
}

#endif // end of BASE_OBJECTS_H_
