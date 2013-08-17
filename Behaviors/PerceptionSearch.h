#ifndef PERCEPTION_SEARCH_H_
#define PERCEPTION_SEARCH_H_

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/VRmixin.h"
#include "Shared/Measures.h"

// C++ Library
#include <cstdlib>

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
    AngSignPi calcBearingFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
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
    float calcDistanceFromAgentToObject(const DualCoding::ShapeRoot& kShape) {
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

    //! Returns the closest shape/object to the agent
    template<typename Type>
    Type getClosestObject(const std::vector<Type>& kObjects)
    {
        const std::size_t kSize = kObjects.size();

        // if the vector's size is zero, return an invalid shape
        if (kSize == 0) {
            return Type();
        }

        // else if the vector's size is one, return the first element (the only shape in the vector)
        else if (kSize == 1) {
            return kObjects[0];
        }

        // else iterate over the vector and find the closest shape
        else {
            Type nearestObject = kObjects[0];
            float nearestObjectDist = calcDistanceFromAgentToObject(nearestObject);
            
            // iterate over the remainder of the objects
            for (std::size_t index = 1; index < kSize; index++) {
                float currentObjectDist = calcDistanceFromAgentToObject(kObjects[index]);
                if (currentObjectDist < nearestObjectDist) {
                    nearestObjectDist = currentObjectDist;
                    nearestObject = kObjects[index];
                }
            }
            return nearestObject;
        }
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