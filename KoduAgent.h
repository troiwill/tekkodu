#ifndef KODU_AGENT_H_
#define KODU_AGENT_H_

// Tekkodu Library
#include "Kodu/KoduPage.h"
#include "Kodu/General/GeneralFncs.h"
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Keepers/ScoreKeeper.h"
#include "Kodu/Primitives/KoduActionMotion.h"

#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"

// Tekkotsu Library
#include "DualCoding/Point.h"
#include "DualCoding/PolygonData.h"
#include "DualCoding/ShapePolygon.h"
#include "DualCoding/VRmixin.h"

// C++ Library
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace Kodu {

    class PerceptualTaskBase;
    
    class KoduAgent {
    public:
        //! Constructor
        KoduAgent()
          : gripperObject(),
            agentIsAttemptingGrab(false),
            targetObjectIsInGripper(false),
            agentIsWalking(false),
            currMotionCmd(),

            distanceTravelled(0.0f),

            pages(),
            currPageIndex(0),
            newReqdPage(0),
            ptasks(),
            scoreQueue(),
            stringToSpeak(""),
            playQueue(),
            agentGazePolygon()
        {
            // generate the gaze polygon
            generateGazePolygon();
        }

        //! Destructor
        ~KoduAgent() {
            GeneralFncs::destroyAllPtrsInQueue(ptasks);
            GeneralFncs::destroyAllPtrsInVector(pages);
            stringToSpeak.clear();
        }

        //! Assignment operator
        KoduAgent& operator=(const KoduAgent& kAgent) {
            if (this != &kAgent) {
                gripperObject = kAgent.gripperObject;
                agentIsAttemptingGrab = kAgent.agentIsAttemptingGrab;
                targetObjectIsInGripper = kAgent.targetObjectIsInGripper;
                agentIsWalking = kAgent.agentIsWalking;
                currMotionCmd = kAgent.currMotionCmd;

                distanceTravelled = kAgent.distanceTravelled;
                
                pages = kAgent.pages;
                currPageIndex = kAgent.currPageIndex;
                newReqdPage = kAgent.newReqdPage;
                ptasks = kAgent.ptasks;
                scoreQueue = kAgent.scoreQueue;
                stringToSpeak = kAgent.stringToSpeak;
                playQueue = kAgent.playQueue;
                agentGazePolygon = kAgent.agentGazePolygon;
            }
            return *this;
        }

        /// ================================ Gazing functions ================================ ///
        //! Returns the gaze polygon (the egocentric points the agent should look at in space)
        const DualCoding::Shape<DualCoding::PolygonData>& getGazePolygon() const;
        
        /// ================================ Grasper functions ================================ ///
        //! States whether or not the agent has an object it wants to grab
        bool hasNewObjectToGrab() const;

        //! Sets the "(agent) is attempting grab" flag
        void setIsAttemptingGrabFlag(bool);

        //! Sets the "target object is in gripper" flag
        void setTargetInGripperFlag(bool);

        //! States if the agent is attempting to grab an object
        bool isAttemptingGrab() const;

        //! States whether or not the agent is (supposed to be) holding something
        bool isHoldingAnObject() const;

        /// ================================ Motion functions ================================ ///
        //! Checks if the agent has a valid motion command
        bool hasMotionCommand() const;

        //! Checks if the agent is walking (returns value of "agent is walking" flag)
        bool isWalking() const;

        //! Checks if the agent needs to localize
        bool needsToLocalize() const;

        /// ================================ Page functions ================================ ///
        //! Returns the page currently being evaluated (determined by current page index variable)
        KoduPage* getCurrentPage() const;

        //! Returns the specified page (using it's page number)
        KoduPage* getPage(unsigned int pageNumber) const;

        //! Returns the specified page (using it's location in the vector--(PAGE# - 1))
        KoduPage* getPageInPos(unsigned int pageIndex) const;

        //! Checks if the agent wants to switch pages
        bool hasNewPageNumber() const;

        /// ================================ Scoring functions ================================ ///
        //! Checks if the agent has a string to speak
        bool hasNewScoreChanges() const;

        /// ================================ Speech functions ================================ ///
        //! Checks if the agent has a string to speak
        bool hasTextToSay() const;

        /// ================================ Sound functions ================================ ///
        //! Checks if the agent has sounds to play
        bool hasSoundsToPlay() const;

    private:
        /// ================================ Gaze functions ================================ ///
        //! Generates the agent's gaze points (the points in space to search for objects)
        void generateGazePolygon();

        // Disallows the copy constructor
        DISALLOW_COPY(KoduAgent);

    public: //// ================= The Public Agent Variables ================= ////
        // === Grasp variables === //
        DualCoding::ShapeRoot gripperObject;//!< The object the agent is holding/will be holding
        bool agentIsAttemptingGrab;     //!< (flag) States if the agent is attempting to grab an object
        bool targetObjectIsInGripper;   //!< (flag) States if the target object is in the gripper

        // === Motion variables === //
        //! The mininum travelling distance (including turns) to consider performing localization
        static const float kLocalizationDistanceThreshold;
        bool agentIsWalking;                //!< A flag stating whether or not the agent is walking
        MotionCommand currMotionCmd;        //!< The current motion command
        float distanceTravelled;
        
        // === Page variables === //
        std::vector<KoduPage*> pages;       //!< The vector of pages containing kode
        unsigned int currPageIndex;         //!< The page (index) currently being executed
        unsigned int newReqdPage;           //!< Stores the page number the agent wants to switch to

        // === Perceptual Tasks container === //
        std::queue<PerceptualTaskBase*> ptasks;

        // === Score variables === //
        std::queue<ScoreChange> scoreQueue; //!< The queue of score operations and their value

        // === Speech variables === //
        std::string stringToSpeak;          //!< The string the agent wants to speak
        
        // === Sound variables === //
        std::queue<std::string> playQueue;  //!< The queue of sound files the agent wants to play

    private: //// ================= The Private Agent Variables ================= ////
        // === Gaze polygon variables === //
        //! egocentric (relative to the robot's body) "directions" to look at (they are really points)
        DualCoding::Shape<DualCoding::PolygonData> agentGazePolygon;
    };
}

#endif // end of KODU_AGENT_H_