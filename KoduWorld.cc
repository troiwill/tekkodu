// Tekkodu Library
#include "Kodu/KoduWorld.h"

// Tekkotsu Library
#include "DualCoding/Point.h"
#include "DualCoding/ShapeFuns.h"

// C++ Library
#include <vector>

namespace Kodu {

    ScoreKeeper KoduWorld::globalScoreKeeper;
    DualCoding::Shape<DualCoding::PointData> KoduWorld::northStarLocation;

    void KoduWorld::applyGlobalScoreChanges(std::queue<ScoreChange>& queue) {
        while (!queue.empty()) {
            const ScoreChange& change = queue.front();
            // apply the appropriate operation
            switch(change.operationType) {
                // add score operation
                case KoduActionScore::ST_SCORE:
                    globalScoreKeeper.addScore(change.designator, change.value);
                    break;

                // set score operation
                case KoduActionScore::ST_SET_SCORE:
                    globalScoreKeeper.setScore(change.designator, change.value);
                    break;

                // subtract score operation
                case KoduActionScore::ST_SUBTRACT:
                    globalScoreKeeper.subtractScore(change.designator, change.value);
                    break;
            }
            // pop the top element off
            queue.pop();
        }
    }

    int KoduWorld::getScoreValue(const std::string& kDesignator) {
        return globalScoreKeeper.checkScoreValue(kDesignator);
    }

    const DualCoding::Shape<DualCoding::PolygonData>& KoduWorld::getWorldBoundsPolygon() {
        return worldBoundsPolygon;
    }

    bool KoduWorld::theNorthStarIsArtificial() const {
        return northStarIsArtifical;
    }
/*
    void KoduWorld::setNorthStar(const DualCoding::Shape<DualCoding::AprilTagData>& kNorthStar,
                                 bool nsIsArtificial)
    {
        northStar = kNorthStar;
        northStarIsArtifical = nsIsArtificial;
        std::cout << "\n\nNorth star location @ point " << northStar->getCentroid() << std::endl;
        std::cout << "North star " << (northStarIsArtifical ? "was artificially created" : "is real")
            << ".\n\n";
    }
*/
    void KoduWorld::setNorthStarLocation(const DualCoding::Shape<DualCoding::PointData>& kNsLocation,
        bool nsIsArtificial)
    {
        northStarLocation = kNsLocation;
        northStarIsArtifical = nsIsArtificial;
        std::cout << "\n\nNorth star location @ point " << northStarLocation->getCentroid() << std::endl;
        std::cout << "North star " << (nsIsArtificial ? "was artificially created" : "is real") << ".\n\n";
    }

    void KoduWorld::generateWorldBoundsPolygon() {
        // create world bounds
        std::vector<DualCoding::Point> worldBounds;
        const float sideLength = 2000.0f;   // assumes the world is a square (all sides are equal)
        const float halfSideLength = sideLength / 2.0f;
        float minX = 0.0f;
        float maxX = 0.0f;
        float minY = 0.0f;
        float maxY = 0.0f;
        minX = -500.0f;
        maxX = sideLength + minX;
        minY = -halfSideLength;
        maxY = halfSideLength;
        // add the points of the square polygon to the vector
        worldBounds.push_back(DualCoding::Point(minX, maxY, 0, DualCoding::egocentric));    // bottom left
        worldBounds.push_back(DualCoding::Point(maxX, maxY, 0, DualCoding::egocentric));    // top left
        worldBounds.push_back(DualCoding::Point(maxX, minY, 0, DualCoding::egocentric));    // top right
        worldBounds.push_back(DualCoding::Point(minX, minY, 0, DualCoding::egocentric));    // bottom right
        worldBounds.push_back(worldBounds[0]);  // close the polygon
        // create a Shape<PolygonData> object and use it as the world bounds.
        NEW_SHAPE(wBoundPolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::worldShS, worldBounds, false));
        worldBoundsPolygon = wBoundPolygon;
        worldBoundsPolygon->setName("worldBoundsPolygon");
    }

}