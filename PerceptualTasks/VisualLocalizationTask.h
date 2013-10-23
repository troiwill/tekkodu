#ifndef VISUAL_LOCALIZATION_TASK_H_
#define VISUAL_LOCALIZATION_TASK_H_

// INCLUDES
// tekkotsu
#include "Crew/PilotRequest.h"
#include "DualCoding/Point.h"
#include "DualCoding/ShapeFuns.h"
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    // tekkodu forward declarations
    class PerceptualTaskBase;
    
    class VisualLocalizationTask : public PerceptualTaskBase {
    public:
        //! Constructor
        VisualLocalizationTask(const DualCoding::Point& kAgentPos, float agentOrient)
          : PerceptualTaskBase(PT_VIS_LOCALIZATION, ++idCount),
            agentPosition(kAgentPos),
            agentOrientation(agentOrientation)
        { }

        //! Copy constructor
        VisualLocalizationTask(const VisualLocalizationTask& kTask)
          : PerceptualTaskBase(kTask),
            agentPosition(kTask.agentPosition),
            agentOrientation(kTask.agentOrientation)
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
            }
            return *this;
        }

        //! Checks if this task can execute
        virtual bool canExecute(const KoduWorld&);

        //! Generates the Pilot request the agent needs to localize
        virtual const DualCoding::PilotRequest& getPilotRequest();

    private:
        static unsigned int idCount;
        DualCoding::Point agentPosition;
        float agentOrientation;
    };
}

#endif // VISUAL_LOCALIZATION_TASK_H_