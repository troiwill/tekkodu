// Tekkodu Library
#include "Kodu/KoduWorld.h"

// Tekkotsu Library
#include "DualCoding/Point.h"
#include "DualCoding/ShapeFuns.h"

// C++ Library
#include <vector>

namespace Kodu {

    ScoreKeeper KoduWorld::globalScoreKeeper;
    
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

    // const DualCoding::ShapeRoot& KoduWorld::getNorthStar() const {
    //     return northStar;
    // }

    const DualCoding::Point& KoduWorld::getNorthStarLocation() const {
        return northStarLocation;
    }

    const DualCoding::Shape<DualCoding::PolygonData>& KoduWorld::getWorldBoundsPolygon() const {
        return worldBoundsPolygon;
    }

/*
    //void KoduWorld::setNorthStar(const DualCoding::ShapeRoot& kNorthStar, bool nsIsArtificial) {
    void KoduWorld::setNorthStar(const DualCoding::ShapeRoot& kNorthStar) {
        if (kNorthStar.isValid() && kNorthStar->getType() == DualCoding::aprilTagDataType) {
            northStarIsArtifical = false;
            northStarLocation = kNorthStar->getCentroid();
        } else {
            northStarIsArtifical = true;
        }
        northStar = kNorthStar;
        //northStarIsArtifical = nsIsArtificial;
        //std::cout << "\n\nNorth star location @ point " << northStar->getCentroid() << std::endl;
        std::cout << "\n\nNorth star location @ point " << northStarLocation << std::endl;
        std::cout << "North star is " << (northStarIsArtifical ? "artificial" : "real") << ".\n\n";
    }
*/

    bool KoduWorld::theNorthStarIsArtificial() const {
        return northStarIsArtifical;
    }

    void KoduWorld::generateWorldBoundsPolygon(const DualCoding::ShapeRoot& kNorthStar) {
        // create world bounds
        std::vector<DualCoding::Point> worldBounds;
        const float kSideLength = 2000.0f;   // assumes the world is a square (all sides are equal)
        const float kHalfSideLength = kSideLength / 2.0f;
        // float minX = 0.0f;
        // float maxX = 0.0f;
        // float minY = 0.0f;
        // float maxY = 0.0f;
        // check if the north star is real (or artificial)
        if (kNorthStar.isValid() && kNorthStar->getType() == DualCoding::aprilTagDataType) {
            // state that the north star is real, and setup the world bounds based on star's location
            northStarIsArtifical = false;
            northStarLocation = DualCoding::Point(kNorthStar->getCentroid());
            
        } else {
            // state that the north star is artificial, and setup generic world bounds
            northStarIsArtifical = true;
            const float kNorthStarGenericMaxX = 1500.0f;
            northStarLocation = DualCoding::Point(kNorthStarGenericMaxX, 0.0f, 0.0f,
                DualCoding::allocentric);
        }
        // create the max and min coordinates of the world bounds
        // const float maxX = northStarLocation.coordX();
        // const float minX = maxX - kSideLength;
        // const float maxY = northStarLocation.coordY() + kHalfSideLength;
        // const float minY = northStarLocation.coordY() - kHalfSideLength; // minY = maxY - kSideLength
        const float maxX = 1500.0f;
        const float minX = maxX - kSideLength;
        const float maxY = kHalfSideLength;
        const float minY = -1.0f * kHalfSideLength; // minY = maxY - kSideLength

        // add the points of the square polygon to the vector
        worldBounds.push_back(DualCoding::Point(minX, maxY, 0, DualCoding::allocentric)); // bottom left
        worldBounds.push_back(DualCoding::Point(maxX, maxY, 0, DualCoding::allocentric)); // top left
        worldBounds.push_back(DualCoding::Point(maxX, minY, 0, DualCoding::allocentric)); // top right
        worldBounds.push_back(DualCoding::Point(minX, minY, 0, DualCoding::allocentric)); // bottom right
        worldBounds.push_back(worldBounds[0]);  // close the polygon
        
        // create a Shape<PolygonData> object and use it as the world bounds.
        NEW_SHAPE(wBoundPolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::worldShS, worldBounds, true));
        worldBoundsPolygon = wBoundPolygon;
        worldBoundsPolygon->setName("worldBoundsPolygon");
        worldBoundsPolygon->setObstacle(true);

        //northStarIsArtifical = nsIsArtificial;
        //std::cout << "\n\nNorth star location @ point " << northStar->getCentroid() << std::endl;
        std::cout << "\n\nNorth star location @ point " << northStarLocation << std::endl;
        std::cout << "North star is " << (northStarIsArtifical ? "artificial" : "real") << ".\n\n";
    }

    /*
    void KoduWorld::generateWorldBoundsPolygon() {
        // create world bounds
        std::vector<DualCoding::Point> worldBounds;
        const float kSideLength = 2000.0f;   // assumes the world is a square (all sides are equal)
        const float kHalfSideLength = kSideLength / 2.0f;
        float minX = 0.0f;
        float maxX = 0.0f;
        float minY = 0.0f;
        float maxY = 0.0f;
        minX = -500.0f;
        maxX = kSideLength + minX;
        minY = -kHalfSideLength;
        maxY = kHalfSideLength;
        // add the points of the square polygon to the vector
        worldBounds.push_back(DualCoding::Point(minX, maxY, 0, DualCoding::egocentric)); // bottom left
        worldBounds.push_back(DualCoding::Point(maxX, maxY, 0, DualCoding::egocentric)); // top left
        worldBounds.push_back(DualCoding::Point(maxX, minY, 0, DualCoding::egocentric)); // top right
        worldBounds.push_back(DualCoding::Point(minX, minY, 0, DualCoding::egocentric)); // bottom right
        worldBounds.push_back(worldBounds[0]);  // close the polygon
        // create a Shape<PolygonData> object and use it as the world bounds.
        NEW_SHAPE(wBoundPolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::worldShS, worldBounds, true));
        worldBoundsPolygon = wBoundPolygon;
        worldBoundsPolygon->setName("worldBoundsPolygon");
    }
    */

}