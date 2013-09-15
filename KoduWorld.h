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
        KoduWorld(const std::string& agentName)
          : thisAgent(agentName, DualCoding::Point(0.0f, 0.0f, 0.0f, DualCoding::egocentric), 0.0f),
            northStarIsArtifical(true),
            worldBoundsPolygon(),
            worldSideLength(2000.0f)    // 2 meters (area = 2m sq)
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

        /// ================================ World Bounds and North Star functions =========== ///
        //! Returns the world bounds polygon
        const DualCoding::Shape<DualCoding::PolygonData>& getWorldBoundsPolygon();

        //! Returns whether or not the north star was seen by the camera
        bool theNorthStarIsArtificial() const;

        //! Sets the "North Star"
        //void setNorthStar(const DualCoding::Shape<DualCoding::AprilTagData>&, bool);

        //! Sets the "North star" location
        void setNorthStarLocation(const DualCoding::Shape<DualCoding::PointData>&, bool);

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

        //! The North Star - used for localization, and defines "north" in the world
        //DualCoding::Shape<DualCoding::AprilTagData> northStar;
        
        //! The North Star - used for localization, and defines "north" in the world
        static DualCoding::Shape<DualCoding::PointData> northStarLocation;

        //! States if the North Star was seen by the camera, or it is artifical
        bool northStarIsArtifical;

        //! The world bounds polygon
        DualCoding::Shape<DualCoding::PolygonData> worldBoundsPolygon;

        //! The length of the world's sides; all sides are the same length making the world a square
        const float worldSideLength;
    };
}

#endif // end of KODU_WORLD_H_