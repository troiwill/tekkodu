#ifndef PERCEPTION_SEARCH_H_
#define PERCEPTION_SEARCH_H_

// Tekkotsu Library
#include "DualCoding/AgentData.h"
#include "DualCoding/ShapeAgent.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    enum SearchRegion_t {
        SRG_UNRESTRICTED    = 0,
        SRG_BEHIND          = 1L << 1,
        SRG_IN_FRONT        = 1L << 2,
        SRG_TO_LEFT         = 1L << 3,
        SRG_TO_RIGHT        = 1L << 4
    };
    
    inline
    SearchRegion_t operator|(SearchRegion_t rs1, SearchRegion_t rs2) {
        return SearchRegion_t(static_cast<int>(rs1) | static_cast<int>(rs2));
    }

    inline
    SearchRegion_t operator|=(SearchRegion_t rs1, SearchRegion_t rs2) {
        return (rs1 = rs1 | rs2);
    }

    inline
    SearchRegion_t operator&(SearchRegion_t rs1, SearchRegion_t rs2) {
        return SearchRegion_t(static_cast<int>(rs1) & static_cast<int>(rs2));
    }

    // TODO
    // Find a permanent solution to removing elements that do not satisfy the predicate.
    // Unary- and binary-predicate function classes are deprecated in C++11
    // Concerns the following lines...
    // Assumes the "Me" shape is always valid
#define PERCEPTION_SEARCH(Dir)                                                  \
    class Is##Dir##Me : public DualCoding::UnaryShapeRootPred {                 \
    public:                                                                     \
        Is##Dir##Me() : DualCoding::UnaryShapeRootPred() { }                    \
                                                                                \
        ~Is##Dir##Me() { }                                                      \
                                                                                \
        bool operator()(const DualCoding::ShapeRoot&) const;                    \
    };

    PERCEPTION_SEARCH(LeftOf);
    PERCEPTION_SEARCH(RightOf);

    PERCEPTION_SEARCH(InFront);
    PERCEPTION_SEARCH(Behind);

    PERCEPTION_SEARCH(CloseBy);
    PERCEPTION_SEARCH(FarAwayFrom);
}

#endif // PERCEPTION_SEARCH_H_