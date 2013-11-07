// INCLUDES
// tekkotsu
#include "Kodu/KoduWorld.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"
#include "Kodu/PerceptualTasks/VisualGripperMonitorTask.h"

namespace Kodu {
    
    float const MatchesObjectTagInGripper::kMaxAprTagCentroidVariance = 30.0f;
    unsigned int VisualGripperMonitorTask::idCount = 70000;
    
    bool MatchesObjectTagInGripper::operator()(const DualCoding::ShapeRoot& kShape) const {
        if (kShape->getType() != DualCoding::aprilTagDataType)
            return false;
        std::cout << "diff in centroids = ";
        float diff = (kShape->getCentroid() - aprTagCentroid).xyNorm();
        std::cout << diff << "mm; ";
        // for debugging
        bool match = ShapeRootTypeConst(kShape, DualCoding::AprilTagData)->getTagID() == aprTagId;
        std::cout << "april tag ids " << (match ? "" : "do not") << " match\n";
        return (diff < kMaxAprTagCentroidVariance && match);
    }

    bool VisualGripperMonitorTask::canExecute(const KoduWorld& kWorldState) {
        return (kWorldState.thisAgent.isHoldingAnObject() && kWorldState.thisAgent.isWalking());
    }

    void VisualGripperMonitorTask::examineTaskResults() {
        std::cout << "Examining visual gripper monitor results...\n";
        DualCoding::Shape<DualCoding::AprilTagData> objectTag
            = DualCoding::find_if<DualCoding::AprilTagData>(DualCoding::VRmixin::localShS, taskPred);

        std::cout << "objectTag reference valid? ";
        if (objectTag.isValid()) {
            lastSuccessfulPos = DualCoding::VRmixin::theAgent->getCentroid();
            lastSuccessfulOrient = DualCoding::VRmixin::theAgent->getOrientation();
            //lastSuccessfulState = posOrientDeque.end();
            std::cout << "yes, it is. state logged at: " << lastSuccessfulPos << " w/ orient = "
                << lastSuccessfulOrient << std::endl;
        } else {
            std::cout << "no, reporting failure.\n";
            // add code to make sure the robot isn't dropping the object when executing this function
            /*
            std::cout << "no, reporting failure. last successful state: ";
            if (lastSuccessfulState != posOrientDeque.end()) {
                std::cout << "pos = " << lastSuccessfulState->position << "; orient = "
                    << lastSuccessfulState->orientation << std::endl;
            } else {
                std::cout << "none recorded!!!\n";
            }
            */
            taskStatus = TS_FAILURE;
        }
    }

    const DualCoding::ShapeRoot& VisualGripperMonitorTask::getGripperObject() const {
        return objInGripper;
    }

    const DualCoding::Point& VisualGripperMonitorTask::getLastSuccessfulPos() const {
        return lastSuccessfulPos;
    }

    float VisualGripperMonitorTask::getLastSuccessfulOrient() const {
        return lastSuccessfulOrient;
    }

    /*
    const PosOrientState& VisualGripperMonitorTask::getLastSuccessfulState() const {
        return lastSuccessfulState;
    }
    */

    const DualCoding::MapBuilderRequest& VisualGripperMonitorTask::getMapBuilderRequest() {
        NEW_SHAPE(tagApproxPosition, DualCoding::PointData,
            new DualCoding::PointData(DualCoding::VRmixin::localShS, taskPred.aprTagCentroid));
        mapreq = DualCoding::MapBuilderRequest(DualCoding::MapBuilderRequest::localMap);
        mapreq.searchArea = tagApproxPosition;
        return mapreq;
    }

    const DualCoding::Point& VisualGripperMonitorTask::getTagCentroid() const {
        return taskPred.aprTagCentroid;
    }

    int VisualGripperMonitorTask::getTagId() const {
        return taskPred.aprTagId;
    }

    bool VisualGripperMonitorTask::taskIsComplete(const KoduWorld& kWorldState) {
        if (taskStatus != TS_IN_PROGRESS)
            return true;

        if (!kWorldState.thisAgent.isHoldingAnObject()) {
            taskStatus = TS_COMPLETE;
            return true;
        }
        return false;
    }
}