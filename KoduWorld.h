#ifndef KODU_WORLD_H_
#define KODU_WORLD_H_

// c++ library
#include <map>

// Tekkodu Library
#include "Kodu/KoduAgent.h"
#include "Kodu/Keepers/ScoreKeeper.h"
#include "Kodu/General/GeneralFncs.h"

// Tekkotsu Library
#include "DualCoding/PolygonData.h"
#include "DualCoding/ShapeAprilTag.h"
#include "DualCoding/ShapePolygon.h"
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    class KoduWorld {
    public:
        //! Constructor
        KoduWorld()
          : thisAgent(),
            northStarLocation(),
            northStarIsArtifical(true),
            starConstellation(),
            worldBoundsPolygon(),
            worldSideLength(2000.0f)    // 2 meters (area = 2m sq)
        { }

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
        //! Returns the world North Star
        const DualCoding::Point& getNorthStarLocation() const;

        //! Returns the "stars" seen and their location
        const std::map<unsigned int, DualCoding::Point>& getStarConstellation() const;

        //! Returns the world bounds polygon
        const DualCoding::Shape<DualCoding::PolygonData>& getWorldBoundsPolygon() const;

        //! Sets the "North Star"
        //void setNorthStar(const DualCoding::ShapeRoot&, bool);
        void generateWorldBoundsPolygon(const DualCoding::ShapeRoot& = DualCoding::ShapeRoot());

        //! Sets the "stars" seen and their allocentric locations
        void setStarConstellation(const std::vector<DualCoding::ShapeRoot>&);

        //! Returns whether or not the north star was seen by the camera
        bool theNorthStarIsArtificial() const;

    private:
        //! Disallows the copy constructor and assignment operator
        DISALLOW_COPY_ASSIGN(KoduWorld);

    public:
        //! Represents the agent that the Kodu Behavior is running on
        KoduAgent thisAgent;

    private:
        //! Local copy of the Score Keeper
        static ScoreKeeper globalScoreKeeper;

        //! The North Star - used for localization, and defines "north" in the world
        //DualCoding::ShapeRoot northStar;
        DualCoding::Point northStarLocation;
        
        //! States if the North Star was seen by the camera, or it is artifical
        bool northStarIsArtifical;

        //! A map of the april tag (used as stars) and their allocentric positions
        std::map<unsigned int, DualCoding::Point> starConstellation;

        //! The world bounds polygon
        DualCoding::Shape<DualCoding::PolygonData> worldBoundsPolygon;

        //! The length of the world's sides; all sides are the same length making the world a square
        const float worldSideLength;
    };
}

#endif // end of KODU_WORLD_H_