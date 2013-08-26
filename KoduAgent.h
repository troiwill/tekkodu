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
          : pages(),
            currPageIndex(0),
            stringToSpeak(""),
            playQueue(),
            agentIsMoving(false),
            approxDistanceTravelled(0.0f),
            currMotionCmd(),
            gazePolygon()
        {
            // generate the gaze polygon
            generateGazePolygon();
        }

        //! Destructor
        ~KoduAgent() {
            pages.clear();
            stringToSpeak.clear();
            playQueue.clear();
        }

        //! Returns the page currently being evaluated (determined by the current page index variable)
        KoduPage* getCurrentPage() const;

        //! Returns the specified page (using it's page number)
        KoduPage* getPage(unsigned int pageNumber) const;

        //! Returns the specified page (using it's location in the vector--(PAGE# - 1))
        KoduPage* getPageInPos(unsigned int pageIndex) const;

        //! Checks if the agent needs to localize
        bool needsToLocalize() const;

    private:
        //! Generates the agent's gaze points (the points in space the agent should search for objects)
        void generateGazePoints();

        // Disallows the copy constructor and assignment operator
        DISALLOW_COPY_ASSIGN(KoduAgent);

        // page stuff
        std::vector<KoduPage*> pages;       //!< The vector of pages containing kode
        unsigned int currPageIndex;         //!< The page (index) currently being executed
        // speech
        std::string stringToSpeak;          //!< The string the agent wants to speak
        // sounds
        std::queue<std::string> playQueue;  //!< The vector of sound files the agent wants to play
        // agent motion
        bool agentIsMoving;                 //!< A flag stating whether or not the agent is moving
        float approxDistanceTravelled;      //!< The approx. distance the agent has travelled (including turns)
        float distanceSinceLastLocalization;//!< The (approx.) distance travelled since the last localization
        MotionCommand currMotionCmd;        //!< The current motion command
        //! egocentric (relative to the robot's body) "directions" to look at (they are really points)
        DualCoding::Shape<DualCoding::PolygonData> gazePolygon;  
    };
}

#endif // end of KODU_AGENT_H_