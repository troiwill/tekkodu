#ifndef VISUAL_LOCALIZATION_TASK_H_
#define VISUAL_LOCALIZATION_TASK_H_

// INCLUDES
// tekkotsu
#include "Crew/PilotRequest.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapePoint.h"
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    // tekkodu forward declarations
    class PerceptualTaskBase;
    
    class VisualLocalizationTask : public PerceptualTaskBase {
    public:
        //! Constructor
        VisualLocalizationTask(const std::map<unsigned int, DualCoding::Point>& kStarConstellation)
          : PerceptualTaskBase(PT_VIS_LOCALIZATION, ++idCount),
            agentPosition(),
            agentOrientation(),
            globalGazePolygon(),
            localizationPoints(kStarConstellation)
        { }

        //! Constructor
        VisualLocalizationTask(const DualCoding::Point& kAgentPos, float agentOrient)
          : PerceptualTaskBase(PT_VIS_LOCALIZATION, ++idCount),
            agentPosition(kAgentPos),
            agentOrientation(agentOrientation),
            globalGazePolygon(),
            localizationPoints()
        { }

        //! Constructor
        VisualLocalizationTask(const DualCoding::Shape<DualCoding::PolygonData>& kGazePolygon)
          : PerceptualTaskBase(PT_VIS_LOCALIZATION, ++idCount),
            agentPosition(),
            agentOrientation(),
            globalGazePolygon(kGazePolygon),
            localizationPoints()
        { }

        //! Copy constructor
        VisualLocalizationTask(const VisualLocalizationTask& kTask)
          : PerceptualTaskBase(kTask),
            agentPosition(kTask.agentPosition),
            agentOrientation(kTask.agentOrientation),
            globalGazePolygon(kTask.globalGazePolygon),
            localizationPoints(kTask.localizationPoints)
        { }

        //! Destructor
        ~VisualLocalizationTask() {
            // no explicit implementation
        }

        //! Assignment operator
        VisualLocalizationTask& operator=(const VisualLocalizationTask& kTask) {
            if (this != &kTask) {
                PerceptualTaskBase::operator=(kTask);
                agentPosition = kTask.agentPosition;
                agentOrientation = kTask.agentOrientation;
                globalGazePolygon = kTask.globalGazePolygon;
                localizationPoints = kTask.localizationPoints;
            }
            return *this;
        }

        //! Checks if this task can execute
        virtual bool canExecute(const KoduWorld&);

        //! Generates the Pilot request the agent needs to localize
        virtual const DualCoding::PilotRequest& getPilotRequest();

        //! States whether the robot used a gaze polygon passed through the constructor, or a custom one
        bool usedGlobalGazePolygon() const;

    private:
        static unsigned int idCount;
        DualCoding::Point agentPosition;
        float agentOrientation;
        DualCoding::Shape<DualCoding::PolygonData> globalGazePolygon;
        std::map<unsigned int, DualCoding::Point> localizationPoints;
    };
}

#endif // VISUAL_LOCALIZATION_TASK_H_