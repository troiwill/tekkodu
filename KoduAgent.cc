// Tekkodu
#include "Kodu/KoduAgent.h"
#include "Kodu/Primitives/PerceptionSearch.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"

// Tekkotsu
#include "DualCoding/ShapeTypes.h"
#include "Shared/get_time.h"
#include "Shared/mathutils.h"

namespace Kodu {
    /// ================================ Static initializations ================================ ///
    const float KoduAgent::kLocalizationDistanceThreshold = 1000.0f;

    /// ================================ Grasper functions ================================ ///
    bool KoduAgent::isExecutingManipAction() const {
        return agentIsExecutingManipAction;
    }

    bool KoduAgent::isHoldingAnObject() const {
        return targetObjectIsInGripper;
    }

    void KoduAgent::setIsExecutingManipActionFlag(bool bval = true) {
        agentIsExecutingManipAction = bval;
    }

    void KoduAgent::setTargetInGripperFlag(bool bval = true) {
        targetObjectIsInGripper = bval;
    }

    void KoduAgent::setWantsToDropObjectFlag(bool bval = true) {
        agentWantsToDropObject = bval;
    }

    void KoduAgent::setWantsToGrabObjectFlag(bool bval = true) {
        agentWantsToGrabObject = bval;
    }

    bool KoduAgent::wantsToDropObject() const {
        return agentWantsToDropObject;
    }

    bool KoduAgent::wantsToGrabObject() const {
        //return (gripperObject.isValid() && !targetObjectIsInGripper);//&& !agentIsExecutingManipAction);
        return agentWantsToGrabObject;
    }

    void KoduAgent::signalDropActionStart() {
        setIsExecutingManipActionFlag();    // states the robot is executing a manipulation action
        setWantsToDropObjectFlag();         // the robot wants to drop an object
    }

    void KoduAgent::signalGrabActionStart() {
        setIsExecutingManipActionFlag();    // states the robot is executing a manipulation action
        setWantsToGrabObjectFlag();         // the robot wants to grab an object
    }

    void KoduAgent::manipulationComplete() {
        // check the type of manipulation completed
        if (agentWantsToDropObject) {
            setWantsToDropObjectFlag(false);    // the robot no longer "wants to drop an object"
            setTargetInGripperFlag(false);      // the object is no longer in the gripper
        }
        // else, it must be the grab action
        else {
            setWantsToGrabObjectFlag(false);    // the robot no longer "wants to grab an object"
            setTargetInGripperFlag();           // (implicit true) object is in the gripper
        }
        setIsExecutingManipActionFlag(false);   // the manipulation action is no longer executing
    }

    /// ================================ Motion functions ================================ ///
    bool KoduAgent::hasMotionCommand() const {
        return currMotionCmd.isValid();
    }

    bool KoduAgent::isWalking() const {
        return agentIsWalking;
    }

    bool KoduAgent::needsToLocalize() const {
        return (distanceTravelled >= kLocalizationDistanceThreshold);
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
        // SA: the search area (in degrees)--(from -SA/2 to SA/2)
        const float kSearchArea = mathutils::deg2rad(200.0f);
        const float kAngleIncrement = mathutils::deg2rad(25.0f);
        
        float radius = 750.0f;

        float currAngle = -1.0f * kSearchArea / 2.0f;
        while (currAngle <= (kSearchArea / 2.0f)) {
            gazePoints.push_back(DualCoding::Point(
                cos(currAngle) * radius,  // x-value
                sin(currAngle) * radius,  // y-value
                0.0f,                     // z-value
                DualCoding::egocentric    // point is relative to agent body
                ));
            currAngle += kAngleIncrement;
        }

        /*
        // create a polygon search area
        NEW_SHAPE(gazePolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::localShS, gazePoints, false));
        // make sure the polygon is not an obstacle (in the localShS) nor viewable (in the local view)
        gazePolygon->setObstacle(false);
        gazePolygon->setViewable(false);
        // assign gazePolygon to agentaGzePoints
        agentGazePolygon = gazePolygon;
        */
    }
    
}