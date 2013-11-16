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
            localizationPoints(kStarConstellation)
        { }

        //! Copy constructor
        VisualLocalizationTask(const VisualLocalizationTask& kTask)
          : PerceptualTaskBase(kTask),
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
                localizationPoints = kTask.localizationPoints;
            }
            return *this;
        }

        //! Checks if this task can execute
        virtual bool canExecute(const KoduWorld&);

        //! Generates the Pilot request the agent needs to localize
        virtual const DualCoding::PilotRequest& getPilotRequest();

    private:
        static unsigned int idCount;    //!< used to create an id for each task
        std::map<unsigned int, DualCoding::Point> localizationPoints;   //!< a copy of the constellation
    };
}

#endif // VISUAL_LOCALIZATION_TASK_H_