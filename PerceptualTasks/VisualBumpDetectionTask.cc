// INCLUDES
// c++
#include <iostream>

// tekkodu
#include "Kodu/KoduWorld.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"
#include "Kodu/PerceptualTasks/VisualBumpDetectionTask.h"
#include "Kodu/Primitives/KoduConditionBump.h"

// tekkotsu
#include "DualCoding/AprilTagData.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeAprilTag.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    unsigned int VisualBumpDetectionTask::idCount = 10000;

    bool VisualBumpDetectionTask::canExecute(const KoduWorld& kWorldState) {
        // this task can execute if:
        // 1) the agent is near the matching object
        // 2) the agent is not walking
        //**************** temp fix
        return ((condition->agentIsNearMatchingObject(kWorldState.thisAgent.gripperObject))
            && (!kWorldState.thisAgent.isWalking()));
        //****************
        //return ((condition->agentIsNearMatchingObject()) && (!kWorldState.thisAgent.isWalking()));
    }

    void VisualBumpDetectionTask::examineTaskResults() {
        std::cout << "Examining visual bump detection results...";
        // create an AprilTagData shape
        NEW_SHAPE(aprilTag, DualCoding::AprilTagData,
            DualCoding::find_if<DualCoding::AprilTagData>(DualCoding::VRmixin::localShS));
        // check if the shape is valid (if it is then the task was successful)
        if (aprilTag.isValid()) {
            std::cout << "found the tag!\n";
            // confirm the agent saw the object
            condition->setVisualBumpDetection(true);
            // tell the interpreter that it can evaluate the condition again
            condition->setAgentCanUsePrimitive(true);
            // state the task is successful
            taskStatus = TS_SUCCESSFUL;
        }
        // else the task was not successful
        else {
            std::cout << "nothing.\n";
            condition->setVisualBumpDetection(false);
            // state the task is a failure
            taskStatus = TS_FAILURE;
        }
    }

    const DualCoding::MapBuilderRequest& VisualBumpDetectionTask::getMapBuilderRequest() {
        const float kSearchRadius = 250.0f;
        const float kSideAngle = mathutils::deg2rad(10.0f);
        
        std::vector<DualCoding::Point> searchPoints;
        searchPoints.push_back(DualCoding::Point(cos(kSideAngle) * kSearchRadius,
                                                 sin(kSideAngle) * kSearchRadius,
                                                 0.0f, DualCoding::egocentric));

        searchPoints.push_back(DualCoding::Point(cos(-1.0f * kSideAngle) * kSearchRadius,
                                                 sin(-1.0f * kSideAngle) * kSearchRadius,
                                                 0.0f, DualCoding::egocentric));

        searchPoints.push_back(DualCoding::Point(kSearchRadius, 0.0f, 0.0f, DualCoding::egocentric));

        NEW_SHAPE(gazePolygon, DualCoding::PolygonData,
            new PolygonData(DualCoding::VRmixin::localShS, searchPoints, false));
        // create the mapbuilder request
        mapreq = DualCoding::MapBuilderRequest(DualCoding::MapBuilderRequest::localMap);
        mapreq.setAprilTagFamily();
        mapreq.searchArea = gazePolygon;
        return mapreq;
    }
}