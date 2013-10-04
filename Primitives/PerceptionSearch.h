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
    inline
    AngSignPi bearingFromAgentToPoint(const DualCoding::Point& kPoint) {
        float dtheta = 0.0f;
        switch (kPoint.getRefFrameType()) {
            // take the agent's centroid into consideration if the point is allocentric
            case DualCoding::allocentric:
            {
                // calculate the bearing between some point "kPoint" and the agent's position
                // bearing2ThisPoint = arctan(dy/dx)
                const DualCoding::Point& kAgentPt = DualCoding::VRmixin::theAgent->getCentroid();
                float bearing2ThisPoint = (kPoint - kAgentPt).atanYX();
                // subtract the agent's orientation (heading) from the bearing to get the point's angle
                // relative ot the agent
                dtheta = bearing2ThisPoint - DualCoding::VRmixin::theAgent->getOrientation();
                break;
            }
            // simply calculate the arctan of the point...
            case DualCoding::egocentric:
            {
                dtheta = kPoint.atanYX();
                break;
            }
            // handles all other Reference Frame Types...
            default:
                std::cout << "Used an illegal reference frame in bearingFromAgentToPoint(...).\n";
                break;
        }
        // return the angle between the agent and the target point
        return dtheta;
    }

    //! Calcaulate the bearing from the agent's position and orientation to a specified shape/object
    inline
    AngSignPi bearingFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
        return bearingFromAgentToPoint(kShape->getCentroid());
    }

    //! Calculates the distance between the agent and a specified point
    inline
    float distanceFromAgentToPoint(const DualCoding::Point& kPoint) {
        float dx = kPoint.coordX();
        float dy = kPoint.coordY();
        switch (kPoint.getRefFrameType()) {
            // since the point is allocentric, take the agent's centroid into consideration.
            case DualCoding::allocentric:
            {
                // get the agent's point
                DualCoding::Point agentPoint = DualCoding::VRmixin::theAgent->getCentroid();
                // calculate the differences in the shape's and agent's positions
                dx = dx - agentPoint.coordX();
                dy = dy - agentPoint.coordY();
                break;
            }
            // since the point is egocentric, there is nothing more to calculate (the agent's centroid
            // is { 0, 0 }).
            case DualCoding::egocentric:
                break;

            // this handles all other Reference Frame Types...
            default:
                std::cout << "Used an illegal reference frame in distanceFromAgentToPoint(...).\n";
                return (0.0f);
        }
        // return the distance
        return sqrt((dx * dx) + (dy * dy));
    }

    //! Calculates the distance between the agent and a specified shape/object
    inline
    float distanceFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
        return distanceFromAgentToPoint(kShape->getCentroid());
    }
/*
    inline
    float distanceInBetweenAgentAndObject(const DualCoding::Point& kPoint) {
        float distBtwObjects = 0.0f;
        switch (kPoint.getRefFrameType()) {
            case DualCoding::allocentric:
            {

            }

            case DualCoding::egocentric:
            {

            }

            default:
        }
        return distBtwObjects;
    }

    //! 
    inline
    float distanceInBetweenAgentAndObject(const DualCoding::ShapeRoot& kShape) {

    }
*/
    //! Returns the closest shape/object to the agent
    template<typename Type>
    Type getClosestObject(const std::vector<Type>&);

    //! Returns the objects located in the specified region(s) relative to the agent orientation
    template<typename Type>
    std::vector<Type> getObjectsLocated(const std::vector<Type>&, SearchLocation_t);

    //! Returns the objects that are a specified color
    template<typename Type>
    inline
    std::vector<Type> getObjectsWithColor(const std::vector<Type>& kObjects, const std::string& color) {
        return DualCoding::subset(kObjects, DualCoding::IsColor(color));
    }

    //! Returns the closest object that matches the specified criteria
    DualCoding::Shape<DualCoding::CylinderData>
    getClosestObjectMatching(const std::string&, SearchLocation_t);
}

#endif // PERCEPTION_SEARCH_H_
