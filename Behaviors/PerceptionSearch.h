#ifndef PERCEPTION_SEARCH_H_
#define PERCEPTION_SEARCH_H_

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"
#include "Shared/Measures.h"

namespace Kodu {

    //! Specifies what region(s) the agent should focus on when using conditions such as see and bump
    enum SearchRegion_t {
        SRG_UNRESTRICTED    = 0,
        SRG_BEHIND          = 1L << 1,
        SRG_IN_FRONT        = 1L << 2,
        SRG_TO_LEFT         = 1L << 3,
        SRG_TO_RIGHT        = 1L << 4
    };
    
    //! Logical OR operation
    inline
    SearchRegion_t operator|(SearchRegion_t rs1, SearchRegion_t rs2) {
        return SearchRegion_t(static_cast<int>(rs1) | static_cast<int>(rs2));
    }

    //! Logicial OR Equals operation
    inline
    SearchRegion_t operator|=(SearchRegion_t rs1, SearchRegion_t rs2) {
        return (rs1 = rs1 | rs2);
    }

    //! Logical AND operation
    inline
    SearchRegion_t operator&(SearchRegion_t rs1, SearchRegion_t rs2) {
        return SearchRegion_t(static_cast<int>(rs1) & static_cast<int>(rs2));
    }

    //! Calcaulate the bearing from the agent to a specified shape/object
    inline
    AngSignPi calcBearingToShape(const DualCoding::ShapeRoot& kShape) {
        // get the agent's data
        DualCoding::AgentData agentData = DualCoding::VRmixin::theAgent.getData();
        // calculate the bearing between some shape "kShape" and the agent ==> theta = arctan(dy/dx)
        float bearing2ThisShape = (kShape.getData().getCentroid() - agentData.getCentroid()).atanYX();
        // subtract the agent's orientation (heading) from the bearing to get the shape's
        // angle relative to the agent
        AngSignPi dtheta = bearing2ThisShape - agentData.getOrientation();
        return dtheta;
    }

    //! Calculates the distance between the agent and a specified shape/object
    inline
    float calcDistanceToShape(const DualCoding::ShapeRoot& kShape) {
        // get the shape's point
        DualCoding::Point shapePoint = kShape.getData().getCentroid();
        // get the agent's point
        DualCoding::Point agentPoint = DualCoding::VRmixin::theAgent.getData().getCentroid();
        // calculate the differences in the shape's and agent's positions
        float dx = shapePoint.coordX() - agentPoint.coordX();
        float dy = shapePoint.coordY() - agentPoint.coordY();
        // return the distance
        return sqrt((dx * dx) + (dy * dy));
    }

    // TODO
    // Find a permanent solution to removing elements that do not satisfy the predicate.
    // Unary- and binary-predicate function classes are deprecated in C++11
    // Concerns the following lines...
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
}

#endif // PERCEPTION_SEARCH_H_