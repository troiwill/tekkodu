#ifndef PERCEPTUAL_TASK_H_
#define PERCEPTUAL_TASK_H_

namespace Kodu {

    class PerceptualTask {
    public:
        //! Constructor #1
        PerceptualTask(const DualCoding::ShapeRoot& kTargetShape)
          : targetShapes(),
            taskComplete(false)
        {
            targetShapes.push_back(kTargetShape);
        }

        //! Constructor #2
        PerceptualTask(const std::vector<DualCoding::ShapeRoot>& kTargetShapes)
          : targetShapes(kTargetShapes),
            taskComplete(false)
        { }

        //! Destructor
        virtual PerceptualTask() {
            // no explicit implementation
        }

        //! Executes a particular perceptual task (reimplemented in all derived classes)
        virtual void executeTask() = 0;

        //! Checks whether a task is complete
        virtual bool taskIsComplete() const = 0;

    protected:
        std::vector<DualCoding::ShapeRoot> targetShapes;
        bool taskComplete;
    };
}


#endif // PERCEPTUAL_TASK_H_