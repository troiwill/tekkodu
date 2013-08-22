#ifndef KODU_CONDITION_SEE_H_
#define KODU_CONDITION_SEE_H_

// Tekkodu Library
#include "Kodu/Behaviors/KoduCondition.h"
#include "Kodu/Behaviors/PerceptionSearch.h"
#include "Kodu/Keepers/ObjectKeeper.h"

// Tekkotsu Library
#include "DualCoding/CylinderData.h"
#include "DualCoding/ShapeCylinder.h"

namespace Kodu {

    class KoduConditionSee : public KoduCondition {
    public:
        //! Constructor
        KoduConditionSee(bool useNot, const std::string& kObjectType, const std::string& kObjectColor,
            SearchLocation_t locationToSearch)
          : KoduCondition("KoduConditionSee"),
            notModifierEnabled(useNot),
            objType(kObjectType),
            objColor(kObjectColor),
            searchLocation(locationToSearch),
            refdObject()
        { }

        //! Copy constructor
        KoduConditionSee(const KoduConditionSee& kCondition)
          : KoduCondition(kCondition),
            notModifierEnabled(kCondition.notModifierEnabled),
            objType(kCondition.objType),
            objColor(kCondition.objColor),
            searchLocation(kCondition.searchLocation),
            refdObject(kCondition.refdObject)
        { }

        //! Destructor
        ~KoduConditionSee() {
            // no explicit implementation
        }

        //! Assignment operator
        KoduConditionSee& operator=(const KoduConditionSee& kCondition) {
            if (this != &kCondition) {
                KoduCondition::operator=(kCondition);
                notModifierEnabled = kCondition.notModifierEnabled;
                objType = kCondition.objType;
                objColor = kCondition.objColor;
                searchLocation = kCondition.searchLocation;
                refdObject = kCondition.refdObject;
            }
            return *this;
        }

        //! Checks if a specified object was seen
        virtual bool evaluate();

        //! Returns a specified object's color
        const std::string& getObjectColor() const;

        //! Returns a specified object's type
        const std::string& getObjectType() const;

        //! Returns the target object... if none is available, return an invalid object
        const DualCoding::Shape<DualCoding::CylinderData>& getTargetObject();

        //! Used to reinitialize certain variables (e.g. when switching to another page)
        virtual void reinitialize();
        
        //! Prints the attributes of a particular instance
        virtual void printAttrs();

    private:
        bool notModifierEnabled;
        std::string objType;
        std::string objColor;
        SearchLocation_t searchLocation;
        DualCoding::Shape<DualCoding::CylinderData> refdObject;
    };
}

#endif // KODU_CONDITION_SEE_H_