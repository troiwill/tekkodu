#ifndef PERCEPTION_SEARCH_H_
#define PERCEPTION_SEARCH_H_

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/CylinderData.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeCylinder.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/VRmixin.h"
#include "Crew/MapBuilder.h"
#include "Shared/Measures.h"

// C++ Library
#include <cstdlib>

namespace Kodu {

    //! Specifies what region(s) the agent should focus on when using conditions such as see and bump
    enum SearchLocation_t {
        SL_UNRESTRICTED     = 0,
        SL_BEHIND           = 1L << 1,
        SL_IN_FRONT         = 1L << 2,
        SL_TO_LEFT          = 1L << 3,
        SL_TO_RIGHT         = 1L << 4,
        SL_CLOSE_BY         = 1L << 5,
        SL_FAR_AWAY         = 1L << 6
    };
    
    //! Logical OR operation
    inline
    SearchLocation_t operator|(SearchLocation_t rs1, SearchLocation_t rs2) {
        return SearchLocation_t(static_cast<int>(rs1) | static_cast<int>(rs2));
    }

    //! Logicial OR Equals operation
    inline
    SearchLocation_t operator|=(SearchLocation_t rs1, SearchLocation_t rs2) {
        return (rs1 = rs1 | rs2);
    }

    //! Logical AND operation
    inline
    SearchLocation_t operator&(SearchLocation_t rs1, SearchLocation_t rs2) {
        return SearchLocation_t(static_cast<int>(rs1) & static_cast<int>(rs2));
    }

    // TODO
    // Find a permanent solution to removing elements that do not satisfy the predicate.
    // Unary- and binary-predicate function classes are deprecated in C++11
    // Concerns the following lines...
    // Creates the HasAreaGreaterThan class
    class HasAreaGreaterThan : public DualCoding::UnaryShapeRootPred {
    public:
        HasAreaGreaterThan(float mininumAcceptableShapeArea)
          : DualCoding::UnaryShapeRootPred(),
            minAcceptableShapeArea(mininumAcceptableShapeArea)
        { }

        ~HasAreaGreaterThan() { }

        bool operator()(const DualCoding::ShapeRoot&) const;

    private:
        float minAcceptableShapeArea;
    };

    class IsMatchForTargetObject : public DualCoding::UnaryShapeRootPred {
    public:
        IsMatchForTargetObject(const DualCoding::ShapeRoot& kTargetObject)
          : DualCoding::UnaryShapeRootPred(),
            targetObject(kTargetObject)
        { }

        ~IsMatchForTargetObject() { }

        bool operator()(const DualCoding::ShapeRoot&) const;

    private:
        DualCoding::ShapeRoot targetObject;
    };

    class IsLandmark : public DualCoding::UnaryShapeRootPred {
    public:
        IsLandmark()
          : DualCoding::UnaryShapeRootPred()
        { }

        ~IsLandmark() { }

        bool operator()(const DualCoding::ShapeRoot&) const;
    };

    class IsShapeOfType : public DualCoding::UnaryShapeRootPred {
    public:
        IsShapeOfType(DualCoding::ShapeType_t shapeType)
          : DualCoding::UnaryShapeRootPred(),
            targetShapeType(shapeType)
        { }

        ~IsShapeOfType() { }

        bool operator()(const DualCoding::ShapeRoot&) const;

    private:
        DualCoding::ShapeType_t targetShapeType;
    };

    //********** temp fix
    class IsNotExcludedShape : public DualCoding::UnaryShapeRootPred {
    public:
        IsNotExcludedShape(const DualCoding::ShapeRoot& kExecption)
          : DualCoding::UnaryShapeRootPred(),
            excludedShape(kExecption)
        { }

        ~IsNotExcludedShape() { }

        bool operator()(const DualCoding::ShapeRoot&) const;

    private:
        DualCoding::ShapeRoot excludedShape;
    };
    //**********

    // Assumes the "Agent" data is always valid
#define PERCEPTION_SEARCH(Dir)                                                  \
    class Is##Dir##Agent : public DualCoding::UnaryShapeRootPred {              \
    public:                                                                     \
        Is##Dir##Agent() : DualCoding::UnaryShapeRootPred() { }                 \
                                                                                \
        ~Is##Dir##Agent() { }                                                   \
                                                                                \
        bool operator()(const DualCoding::ShapeRoot&) const;                    \
    };

    // Creates IsLeftOfAgent and IsRightOfAgent
    PERCEPTION_SEARCH(LeftOf);
    PERCEPTION_SEARCH(RightOf);

    // Creates IsInFrontOfAgent and IsBehindAgent
    PERCEPTION_SEARCH(InFront);
    PERCEPTION_SEARCH(Behind);

    // Creates IsCloseByAgent and IsFarAwayFromAgent
    PERCEPTION_SEARCH(CloseBy);
    PERCEPTION_SEARCH(FarAwayFrom);

    //! Calculate the bearing from the agent's position and orientation to a specified point
    float bearingFromAgentToPoint(const DualCoding::Point&);

    //! Calcaulate the bearing from the agent's position and orientation to a specified shape/object
    float bearingFromAgentToObject(const DualCoding::ShapeRoot&);

    //! Calculates the distance between the agent and a specified point
    float distanceFromAgentToPoint(const DualCoding::Point&);

    //! Calculates the distance between the agent and a specified shape/object
    float distanceFromAgentToObject(const DualCoding::ShapeRoot&);

    //! 
    float distanceInBetweenAgentAndObject(const DualCoding::ShapeRoot&);

    float objectRadius(const DualCoding::ShapeRoot&);

    //! Returns the closest shape/object to the agent
    template<typename Type>
    Type getClosestObject(const std::vector<Type>&);

    //! Returns the objects located in the specified region(s) relative to the agent orientation
    template<typename Type>
    std::vector<Type> getObjectsLocated(const std::vector<Type>&, SearchLocation_t);

    //! Returns the objects that are a specified color
    template<typename Type>
    inline
    std::vector<Type> getObjectsWithColor(const std::vector<Type>& kObjects, const std::string& kColor) {
        return DualCoding::subset(kObjects, DualCoding::IsColor(kColor));
    }

    //! Returns the closest object that matches the specified criteria
    //*************** temp fix
    DualCoding::Shape<DualCoding::CylinderData>
    getClosestObjectMatching(const std::string&, SearchLocation_t, 
        const DualCoding::ShapeRoot& = DualCoding::ShapeRoot());
    //***************
    /*
    DualCoding::Shape<DualCoding::CylinderData>
    getClosestObjectMatching(const std::string&, SearchLocation_t);
    */
}

#endif // PERCEPTION_SEARCH_H_
