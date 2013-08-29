#ifndef KODU_WORLD_H_
#define KODU_WORLD_H_

// Tekkodu Library
#include "Kodu/KoduAgent.h"
#include "Kodu/Keepers/ScoreKeeper.h"
#include "Kodu/General/GeneralFncs.h"

// Tekkotsu Library
#include "DualCoding/PolygonData.h"
#include "DualCoding/ShapePolygon.h"

namespace Kodu {

    class KoduWorld {
    public:
        //! Constructor
        KoduWorld(const DualCoding::Point& agentLocation, float agentOrientation)
          : thisAgent(agentLocation, agentOrientation),
            worldBoundsPolygon()
        {
            // generates the world bounds polygon
            generateWorldBoundsPolygon();
        }

        //! Destructor
        ~KoduWorld() {
            // reinitialize score board
            globalScoreKeeper.initialize();
        }

        /// ================================ Scoring functions ================================ ///
        //! Applies the scores changes to the global score keeper
        void applyGlobalScoreChanges(std::queue<ScoreChange>&);

        //! Returns the value of a particular score (identified by the designator)
        static int getScoreValue(const std::string&);

        /// ================================ World Bounds functions ================================ ///
        //! Returns the world bounds polygon
        const DualCoding::Shape<DualCoding::PolygonData>& getWorldBoundsPolygon();

    private:
        //! Generates the world bounds
        void generateWorldBoundsPolygon();

        //! Disallows the copy constructor and assignment operator
        DISALLOW_COPY_ASSIGN(KoduWorld);

    public:
        //! Represents the agent that the Kodu Behavior is running on
        KoduAgent thisAgent;

    private:
        //! Local copy of the Score Keeper (each agent has its own copy which all must be kept synchronized)
        static ScoreKeeper globalScoreKeeper;

        //! The world bounds polygon
        DualCoding::Shape<DualCoding::PolygonData> worldBoundsPolygon;
    };
}

#endif // end of KODU_WORLD_H_