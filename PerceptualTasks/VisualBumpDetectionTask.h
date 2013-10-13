#ifndef VISUAL_BUMP_DETECTION_TASK_H_
#define VISUAL_BUMP_DETECTION_TASK_H_

// INCLUDES
// tekkotsu
#include "DualCoding/ShapeRoot.h"
#include "DualCoding/VRmixin.h"

#include "Kodu/Primitives/KoduConditionBump.h"

namespace Kodu {
    // tekkodu forward declarations
    //class KoduConditionBump;
    class KoduWorld;
    class PerceptualTaskBase;
    
    class VisualBumpDetectionTask : public PerceptualTaskBase {
    public:
        //! Constructor
        VisualBumpDetectionTask(KoduConditionBump* bmpCondition)
          : PerceptualTaskBase(PT_VIS_BUMP_DETECTION, ++idCount),
            condition(bmpCondition)
        {
            // create the gaze point (look directly down)
            NEW_SHAPE(gazePoint, DualCoding::PointData,
                new PointData(DualCoding::VRmixin::localShS,
                    DualCoding::Point(200, 0, 0, DualCoding::egocentric)));
            // create the mapbuilder request
            mapreq = DualCoding::MapBuilderRequest(DualCoding::MapBuilderRequest::localMap);
            mapreq.setAprilTagFamily();
            mapreq.searchArea = gazePoint;
            std::cout << "Created task #" << id << " to detect a(n) " << condition->getObjectColor()
                      << " " << condition->getObjectType() << std::endl;
        }

        //! Copy constructor
        VisualBumpDetectionTask(const VisualBumpDetectionTask& kTask)
          : PerceptualTaskBase(kTask),
            condition(kTask.condition)
        { }

        //! Destructor
        ~VisualBumpDetectionTask() {
            condition = NULL;
        }

        //! Assignment operator
        VisualBumpDetectionTask& operator=(const VisualBumpDetectionTask& kTask) {
            if (this != &kTask) {
                PerceptualTaskBase::operator=(kTask);
                condition = kTask.condition;
            }
            return *this;
        }

        //! Checks if the VisualBumpDetectionTask can execute
        virtual bool canExecute(const KoduWorld&);

        //! Examines the results from the MapBuilder request to see if an object was "bumped"
        virtual void examineTaskResults();

    private:
        static unsigned int idCount;    //!< Used to generate id numbers for VisualBumpDetectionTask
        KoduConditionBump* condition;   //!< The bump condition that needs to detect the bump
    };
}

#endif // VISUAL_BUMP_DETECTION_TASK_H_