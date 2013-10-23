// INCLUDES
// c++
#include <cmath>
#include <sstream>

// tekkodu
#include "Kodu/KoduWorld.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"
#include "Kodu/PerceptualTasks/VisualWalkProgressTask.h"
#include "Kodu/Primitives/PerceptionSearch.h"

// tekkotsu
#include "DualCoding/VRmixin.h"

namespace Kodu {

    unsigned int const VisualWalkProgressTask::kMaxErrorOccurences = 1;
    unsigned int VisualWalkProgressTask::idCount = 30000;

    bool VisualWalkProgressTask::canExecute(const KoduWorld& kWorldState) {
        return kWorldState.thisAgent.isWalking();
    }

    void VisualWalkProgressTask::examineTaskResults() {
        // get all the objects in the local shape space
        std::vector<DualCoding::ShapeRoot> lclShapes(DualCoding::VRmixin::localShS);
        // import the target shape into the local shape space
        DualCoding::ShapeRoot lclTarget = DualCoding::VRmixin::mapBuilder->importWorldToLocal(targets[0]);
        // iterate over all the shapes in the local shape space for a match
        size_t const kNumbOfShapes = lclShapes.size();
        std::cout << "Examining Walk Progress...";
        for (size_t i = 0; i < kNumbOfShapes; i++) {
            if (lclShapes[i]->isMatchFor(lclTarget)) {
                std::cout << "found a match!\n";
                errorCount = 0;
                return;
            }
        }

        // ***NOTE: execution of the remaining portion of this function means a mathc was not found
        // check if the robot turned; sometimes that can cause an error (no match)
        const float kFiveDegrees = 3.0f * M_PI / 180.0f;
        float agentCurrentOrientation = DualCoding::VRmixin::theAgent->getOrientation();
        if (std::abs(AngSignPi(agentLastOrientation - agentCurrentOrientation)) > kFiveDegrees) {
            std::cout << "Turning may have caused a matching error... ignoring error\n";
            return;
        }

        // increment the error count since the robot did not correctly identify the object
        errorCount++;
        std::stringstream stream;
        stream << "task #" << id << ": recording (error) strike #" << errorCount << ". ";
        // record the agent's position when error occurred
        if (errorCount == 1) {
            agentPosDuringFirstError = DualCoding::VRmixin::theAgent->getCentroid();
            agentOrientationDuringFirstError = DualCoding::VRmixin::theAgent->getOrientation();
            stream << "Recorded robot's position @ " << agentPosDuringFirstError << ".";
        }

        if (errorCount == kMaxErrorOccurences) {
            taskStatus = TS_FAILURE;
            stream << " Task failed!";
        }
        std::cout << stream.str() << std::endl;
    }

    const DualCoding::MapBuilderRequest& VisualWalkProgressTask::getMapBuilderRequest() {

        // save the agent's current orientation
        agentLastOrientation = DualCoding::VRmixin::theAgent->getOrientation();

        mapreq = DualCoding::MapBuilderRequest(DualCoding::MapBuilderRequest::localMap);
        const DualCoding::Point& kTargetPt = targets[0]->getCentroid();
        const DualCoding::Point& kAgentPt = DualCoding::VRmixin::theAgent->getCentroid();
        //const float kAgentOrientation = DualCoding::VRmixin::theAgent->getOrientation();
        mapreq.addObjectColor(targets[0]->getType(), targets[0]->getColor());
        float dtheta = bearingFromAgentToPoint(kTargetPt);
        float h = kTargetPt.xyDistanceFrom(kAgentPt);
        float x = cos(dtheta) * h;
        float y = sin(dtheta) * h;
        NEW_SHAPE(gazePoint, PointData,
            new PointData(DualCoding::VRmixin::localShS,
                DualCoding::Point(x, y, 0, DualCoding::egocentric)));
        mapreq.searchArea = gazePoint;
        return mapreq;
    }

    float VisualWalkProgressTask::getOrientationDuringError() const {
        return agentOrientationDuringFirstError;
    }

    const DualCoding::Point& VisualWalkProgressTask::getPositionDuringError() const {
        return agentPosDuringFirstError;
    }

    bool VisualWalkProgressTask::taskIsComplete(const KoduWorld& kWorldState) {
        switch (taskStatus) {
            case PerceptualTaskBase::TS_IN_PROGRESS:
            {
                if (!kWorldState.thisAgent.isWalking()) {
                    taskStatus = TS_SUCCESSFUL;
                    return true;
                }
                return false;
            }

            case PerceptualTaskBase::TS_SUCCESSFUL:
            case PerceptualTaskBase::TS_FAILURE:
            case PerceptualTaskBase::TS_COMPLETE:
                return true;

            default:
                return false;
        }
    }
}