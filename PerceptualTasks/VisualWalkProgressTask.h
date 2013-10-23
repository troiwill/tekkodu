#ifndef VISUAL_WALK_PROGRESS_TASK_H_
#define VISUAL_WALK_PROGRESS_TASK_H_

// INCLUDES
// c++
#include <vector>

// tekkotsu
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    // tekkodu forward declarations
    class PerceptualTaskBase;

    /**
     * ASSUMPTIONS:
     * - The target shape passed to the constructor is in the world shape space.
     *
    **/
    class VisualWalkProgressTask : public PerceptualTaskBase {
    public:
        //! Constructor #1
        VisualWalkProgressTask(const DualCoding::ShapeRoot& kTargetShape)
          : PerceptualTaskBase(PT_VIS_WALK_PROGRESS, ++idCount),
            errorCount(0),
            agentPosDuringFirstError(),
            agentOrientationDuringFirstError(0.0f),
            targets(),

            firstRun(true),
            agentLastOrientation(0.0f)
        {
            targets.push_back(kTargetShape);
            std::cout << "Created task #" << id << " to track " << kTargetShape << std::endl;
        }

        //! Constructor #2
        VisualWalkProgressTask(const std::vector<DualCoding::ShapeRoot>& kTargetShapes)
          : PerceptualTaskBase(PT_VIS_WALK_PROGRESS, ++idCount),
            errorCount(0),
            agentPosDuringFirstError(),
            agentOrientationDuringFirstError(0.0f),
            targets(kTargetShapes),

            firstRun(true),
            agentLastOrientation(0.0f)
        { }

        //! Copy constructor
        VisualWalkProgressTask(const VisualWalkProgressTask& kTask)
          : PerceptualTaskBase(kTask),
            errorCount(kTask.errorCount),
            agentPosDuringFirstError(kTask.agentPosDuringFirstError),
            agentOrientationDuringFirstError(kTask.agentOrientationDuringFirstError),
            targets(kTask.targets),

            firstRun(kTask.firstRun),
            agentLastOrientation(kTask.agentLastOrientation)
        { }

        //! Destructor
        ~VisualWalkProgressTask() {
            // no explicit implementation
        }

        //! Assignment operator
        VisualWalkProgressTask& operator=(const VisualWalkProgressTask& kTask) {
            if (this != &kTask) {
                PerceptualTaskBase::operator=(kTask);
                errorCount = kTask.errorCount;
                agentPosDuringFirstError = kTask.agentPosDuringFirstError;
                agentOrientationDuringFirstError = kTask.agentOrientationDuringFirstError;
                targets = kTask.targets;

                firstRun = kTask.firstRun;
                agentLastOrientation = kTask.agentLastOrientation;
            }
            return *this;
        }

        //! Checks if the VisualWalkProgressTask can execute
        virtual bool canExecute(const KoduWorld&);

        //! Examines the results from the MapBuilder request to see if the robot made "progress"
        virtual void examineTaskResults();

        //! Dynamically generates the point the agent should fixate on
        virtual const DualCoding::MapBuilderRequest& getMapBuilderRequest();

        //! Returns the agent's orientation when the lastest first error occurred
        float getOrientationDuringError() const;

        //! Returns the agent's position when the lastest first error occurred
        const DualCoding::Point& getPositionDuringError() const;

        //! Checks if the task is complete
        virtual bool taskIsComplete(const KoduWorld&);

        //! The max number of cumulative errors (error: when the agent did not find a match)
        static unsigned int const kMaxErrorOccurences;
    private:
        static unsigned int idCount;                //!< Used to generate id numbers
        unsigned int errorCount;                    //!< Cumulative error count
        DualCoding::Point agentPosDuringFirstError; //!< Position of agent at each "first" error
        float agentOrientationDuringFirstError;     //!< Orientation of agent at each "first" error 
        std::vector<DualCoding::ShapeRoot> targets; //!< The shapes the agent needs to look at
        bool firstRun;
        float agentLastOrientation;
    };

}

#endif // VISUAL_WALK_PROGRESS_TASK_H_
