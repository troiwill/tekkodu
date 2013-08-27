#ifndef KODU_AGENT_H_
#define KODU_AGENT_H_

// Tekkodu Library
#include "Kodu/KoduPage.h"
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Primitives/KoduActionMotion.h"

// Tekkotsu Library
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

    class KoduAgent {
    public:
        //! Constructor
        KoduAgent()
          : isWalkingFlag(false),
            currMotionCmd(),
            newReqdPage(0),
            stringToSpeak(""),
            playQueue(),
            pages(),
            currPageIndex(0),
            walkStartTime(0),
            totalApproxDistanceTravelled(0.0f),
            distanceSinceLastLocalization(0.0f),
            agentGazePolygon()
        {
            // generate the gaze polygon
            generateGazePolygon();
        }

        //! Destructor
        ~KoduAgent() {
            pages.clear();
            stringToSpeak.clear();
        }

        /// ================================ Gazing functions ================================ ///
        //! Returns the gaze polygon (the egocentric points the agent should look at in space)
        const DualCoding::Shape<DualCoding::PolygonData>& getGazePolygon() const;
        
        /// ================================ Motion functions ================================ ///
        //! Checks if the agent has a valid motion command
        bool hasMotionCommand() const;

        //! Checks if the agent is walking (returns value of isWalkingFlag)
        bool isWalking() const;

        //! Checks if the agent needs to localize
        bool needsToLocalize() const;

        //! Notes what time the agent started walking and sets the "walking" flag to true
        void startMonitoringWalk();

        //! Calculates the approx distance the robot travelled and sets the "walking" flag to false
        void stopMonitoringWalk();

        /// ================================ Page functions ================================ ///
        //! Returns the page currently being evaluated (determined by the current page index variable)
        KoduPage* getCurrentPage() const;

        //! Returns the specified page (using it's page number)
        KoduPage* getPage(unsigned int pageNumber) const;

        //! Returns the specified page (using it's location in the vector--(PAGE# - 1))
        KoduPage* getPageInPos(unsigned int pageIndex) const;

        //! Checks if the agent wants to switch pages
        bool hasNewPageNumber() const;

        /// ================================ Speech functions ================================ ///
        //! Checks if the agent has a string to speak
        bool hasTextToSay() const;

        /// ================================ Sound functions ================================ ///
        //! Checks if the agent has sounds to play
        bool hasSoundsToPlay() const;

        //! States whether or not the agent is (supposed to be) holding something
        // bool isHoldingAnObject() const;

    private:
        /// ================================ Gaze functions ================================ ///
        //! Generates the agent's gaze points (the points in space the agent should search for objects)
        void generateGazePolygon();

        // Disallows the copy constructor and assignment operator
        DISALLOW_COPY_ASSIGN(KoduAgent);

    public: //// ================= The Public Agent Variables ================= ////
        // === Motion variables === //
        //! The mininum travelling distance (including turns) to consider performing localization
        static const float kLocalizationDistanceThreshold;
        bool isWalkingFlag;                 //!< A flag stating whether or not the agent is walking
        MotionCommand currMotionCmd;        //!< The current motion command
        
        // === Page variables === //
        unsigned int newReqdPage;           //!< Stores the page number the agent wants to switch to

        // === Speech variables === //
        std::string stringToSpeak;          //!< The string the agent wants to speak
        
        // === Sound variables === //
        std::queue<std::string> playQueue;  //!< The vector of sound files the agent wants to play
        
        //! The object the agent is holding
        // DualCoding::ShapeRoot heldObject;

    //private: //// ================= The Private Agent Variables ================= ////
        // === Page variables === //
        std::vector<KoduPage*> pages;       //!< The vector of pages containing kode
        unsigned int currPageIndex;         //!< The page (index) currently being executed
    
    private:
        // === Motion variables === //
        unsigned int walkStartTime;         //!< The time (in milliseconds) the agent started walking
        float totalApproxDistanceTravelled; //!< The total approx. distance travelled (including turns)
        float distanceSinceLastLocalization;//!< The (approx.) distance travelled since the last localization
        
        // === Gaze polygon variables === //
        //! egocentric (relative to the robot's body) "directions" to look at (they are really points)
        DualCoding::Shape<DualCoding::PolygonData> agentGazePolygon;
    };
}

#endif // end of KODU_AGENT_H_