// INCLUDES
// c++
#include <iostream>
#include <vector>

// tekkodu
#include "Kodu/KoduWorld.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"
#include "Kodu/PerceptualTasks/VisualLocalizationTask.h"
#include "Kodu/Primitives/PerceptionSearch.h"

// tekkotsu
#include "Crew/MapBuilderRequest.h"
#include "DualCoding/PolygonData.h"
#include "DualCoding/VRmixin.h"

namespace Kodu {

    unsigned int VisualLocalizationTask::idCount = 50000;

    bool VisualLocalizationTask::canExecute(const KoduWorld& kWorldState) {
        return (!kWorldState.thisAgent.isWalking());
    }
    
    const DualCoding::PilotRequest& VisualLocalizationTask::getPilotRequest() {
        std::cout << "[Visual Localization Task]\n";
        const unsigned int kMinAmountOfShapesToLocalize = 2;
        DualCoding::MapBuilderRequest* mreq = NULL;
        mreq = new DualCoding::MapBuilderRequest(DualCoding::MapBuilderRequest::localMap);

        if (!localizationPoints.empty()) {
            std::vector<DualCoding::Point> starLocs;
            starLocs.reserve(localizationPoints.size());
            std::cout << "creating the localization point vector for " << localizationPoints.size()
                << " points.\n";

            const int kNumbOfStarsNeeded = 2;
            int numbOfStarsInVector = 0;
            for (std::map<unsigned int, DualCoding::Point>::iterator it = localizationPoints.begin();
                it != localizationPoints.end(); ++it)
            {
                //******** to fix... make the robot only look a little back when stars are to the side
                AngSignPi dtheta = bearingFromAgentToPoint(it->second);
                dtheta += AngSignPi(M_PI / 2.0f);
                if (dtheta > 0.0f) {
                    count++;
                    std::cout << "adding tag #" << it->first << " @ " << it->second << " to vector.\n";
                    NEW_SHAPE(star, DualCoding::AprilTagData,
                        new DualCoding::AprilTagData(DualCoding::VRmixin::worldShS,
                            AprilTags::TagDetection(it->first), DualCoding::Point(it->second)));
                    DualCoding::Point starCen(star->getCentroid());
                    starLocs.push_back(starCen);
                    starLocs.push_back(DualCoding::Point(starCen.coordX() - 200.0f, starCen.coordY(),
                        starCen.coordZ(), starCen.getRefFrameType()));
                    
                    if ((++numbOfStarsInVector) == kNumbOfStarsNeeded) break;
                }
                //**********
            }
            std::cout << "generating localization polygon\n";
            NEW_SHAPE(localizePolygon, DualCoding::PolygonData,
                new DualCoding::PolygonData(DualCoding::VRmixin::localShS, starLocs, false));
            localizePolygon->setObstacle(false);
            localizePolygon->setViewable(true);

            std::cout << "creating mapbuilder request\n";
            mreq->setAprilTagFamily();
            mreq->searchArea = localizePolygon;

        } else if (!globalGazePolygon.isValid()) {
            std::cout << "getting the usable objects in the world\n";
            std::vector<DualCoding::ShapeRoot> usableShapes;
            usableShapes = DualCoding::subset(DualCoding::VRmixin::worldShS,
                                              IsShapeOfType(cylinderDataType));

            // calculate the shapes' positions relative to the robot
            std::vector<DualCoding::Point> localizeGazePoints;
            std::cout << "generating localization points from shapes\n";
            for (std::size_t i = 0; i < usableShapes.size(); i++) {
                DualCoding::ShapeRoot& currShape = usableShapes[i];
                const DualCoding::Point& kCurrShapePt = currShape->getCentroid();
                // calculate the bearing to the shape
                //float bearingToCurrShape = (kCurrShapePt - agentPosition).atanYX();
                //float dtheta = AngSignPi(bearingToCurrShape - agentOrientation);
                // calculate the distance to the shape's centroid
                float dx = kCurrShapePt.coordX() - agentPosition.coordX();
                float dy = kCurrShapePt.coordY() - agentPosition.coordY();
                //float h = sqrt((dx * dx) + (dy * dy));
                localizeGazePoints.push_back(DualCoding::Point(dx, dy, 0.0f, DualCoding::egocentric));
            }
            
            // create the polygon shape
            std::cout << "generating localization polygon\n";
            NEW_SHAPE(localizePolygon, DualCoding::PolygonData,
                new DualCoding::PolygonData(DualCoding::VRmixin::localShS, localizeGazePoints, false));
            localizePolygon->setObstacle(false);
            localizePolygon->setViewable(true);

            // build the mapbuilder request
            std::cout << "creating mapbuilder request\n";
            for (std::size_t i = 0; i < usableShapes.size(); i++) {
                mreq->addObjectColor(usableShapes[i]->getType(), usableShapes[i]->getColor());
            }
            mreq->searchArea = localizePolygon;
        } else {
            mreq->searchArea = globalGazePolygon;
        }
        //mreq->pursueShapes = true;
        mreq->removePts = false;
        
        // create the pilot request
        std::cout << "creating pilot request\n";
        pilotreq = DualCoding::PilotRequest(DualCoding::PilotTypes::localize);
        pilotreq.landmarkExtractor = mreq;
        
        // return the pilot request
        taskStatus = TS_COMPLETE;
        std::cout << "done!\n";
        mreq = NULL;
        return pilotreq;
    }

    bool VisualLocalizationTask::usedGlobalGazePolygon() const {
        return globalGazePolygon.isValid();
    }
}