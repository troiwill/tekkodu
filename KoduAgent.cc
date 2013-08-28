// Tekkodu
#include "Kodu/KoduAgent.h"

// Tekkotsu
#include "DualCoding/Point.h"
#include "DualCoding/ShapeTypes.h"
#include "Shared/get_time.h"
#include "Shared/mathutils.h"

namespace Kodu {
    /// ================================ Static initializations ================================ ///
    const float KoduAgent::kLocalizationDistanceThreshold = 3000.0f;

    /// ================================ Motion functions ================================ ///
    bool KoduAgent::hasMotionCommand() const {
        return currMotionCmd.isValid();
    }

    bool KoduAgent::isWalking() const {
        return isWalkingFlag;
    }

    bool KoduAgent::needsToLocalize() const {
        return ((distanceSinceLastLocalization >= kLocalizationDistanceThreshold)
                && getCurrentPage()->requiresVision());
    }

    void KoduAgent::startMonitoringWalk() {
        isWalkingFlag = true;
        walkStartTime = get_time();
    }

    void KoduAgent::stopMonitoringWalk() {
        isWalkingFlag = false;
        unsigned int timeElasped = get_time() - walkStartTime;
        walkStartTime = 0;
        // if the time is less than the specified value, do not do the calculation
        // the agent probably did not have enough time to accelerate/walk
        if (timeElasped > 200) {
            // calculate the approx distance travelled, and add it to:
            // 1) the total approx distance and
            // 2) the distance since last localization
            float approxDistanceTravelled = KoduActionMotion::kWalkingSpeed * (timeElasped / 1000);
            totalApproxDistanceTravelled += approxDistanceTravelled;
            distanceSinceLastLocalization += approxDistanceTravelled;
            std::cout << "Approx. Dist. Travelled = " << approxDistanceTravelled << std::endl;
            std::cout << "Total Approx. Dist. Travelled = " << totalApproxDistanceTravelled << std::endl;
            std::cout << "Dist. Since Last Localization = " << distanceSinceLastLocalization << std::endl;
        }
    }

    /// ================================ Scoring functions ================================ ///
    bool KoduAgent::hasNewScoreChanges() const {
        return (!scoreQueue.empty());
    }

    /// ================================ Page functions ================================ ///
    KoduPage* KoduAgent::getCurrentPage() const {
        return pages[currPageIndex];
    }

    KoduPage* KoduAgent::getPage(unsigned int pageNumber) const {
        return getPageInPos(pageNumber - 1);
    }

    KoduPage* KoduAgent::getPageInPos(unsigned int pageIndex) const {
        if (pageIndex < pages.size()) {
            return pages[pageIndex];
        } else {
            std::cout << "Page index \"" << pageIndex << "\" is out of bounds!\nReturning NULL...\n";
            return NULL;
        }
    }

    bool KoduAgent::hasNewPageNumber() const {
        return (newReqdPage > 0);
    }

    /// ================================ Speech functions ================================ ///
    bool KoduAgent::hasTextToSay() const {
        return (!stringToSpeak.empty());
    }

    /// ================================ Sound functions ================================ ///
    bool KoduAgent::hasSoundsToPlay() const {
        return (!playQueue.empty());
    }

    /// ================================ Gaze functions ================================ ///
    const DualCoding::Shape<DualCoding::PolygonData>& KoduAgent::getGazePolygon() const {
        return agentGazePolygon;
    }

    void KoduAgent::generateGazePolygon() {
        // SA: the search area (in degrees)
        const float kSearchArea = 160.0f;
        // FoV: the (reduced) field of view
        // (reduced to compensate for any inaccuracies in moving the head)
        // the actual field of view is approx. 75 degrees
        const float kFoV = 70.0f;
        // N_GA: the minimum number of times the robot will have to move it's head to view the search area
        int numbOfGazeAngles = ceil(kSearchArea / kFoV);
        // delta_GA: the change in angle each time the robot moves it's head
        float deltaGazeAngle = ceil(kSearchArea / numbOfGazeAngles);
        // GA<N_GA>: the vector of gaze angles
        std::vector<float> gazeAngles;
        // GA[0]: (-SA / 2) + (delta_GA / 2)
        gazeAngles.push_back((-1 * (kSearchArea / 2)) + (deltaGazeAngle / 2));
        // GA[i]: GA[i - 1] + delta_GA
        for (int i = 1; i < numbOfGazeAngles; i++)
            gazeAngles.push_back(gazeAngles[i - 1] + deltaGazeAngle);

        // create the gaze points using the gaze angles
        std::vector<DualCoding::Point> gazePoints;
        const int numbOfRadii = 2;
        float radii[numbOfRadii] = {
            2000.0f,        // the distant radius
             500.0f         // the nearby radius
        };

        for (int radiiIndex = 0; radiiIndex < numbOfRadii; radiiIndex++) {
            for (int gaIndex = 0; gaIndex < numbOfGazeAngles * 2; gaIndex++) {
                gazePoints.push_back(DualCoding::Point(
                    cos(mathutils::deg2rad(gazeAngles[gaIndex])) * radii[radiiIndex],  // x-value
                    sin(mathutils::deg2rad(gazeAngles[gaIndex])) * radii[radiiIndex],  // y-value
                    0.0f,                                                              // z-value
                    DualCoding::egocentric                     // point is relative to agent body
                    ));
            }
        }
        // create a polygon search area
        NEW_SHAPE(gazePolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::worldShS, gazePoints, false));
        // make sure the polygon is not an obstacle (in the worldShS) nor viewable (in the world view)
        gazePolygon->setObstacle(false);
        gazePolygon->setViewable(false);
        // assign gazePolygon to agentaGzePoints
        agentGazePolygon = gazePolygon;
    }
    
}