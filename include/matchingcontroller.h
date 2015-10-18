/*Copywrite 2014 Shaun Bond, Jeffery Waldon, Charlie Carlson, Simon Smith, Joseph DeWilde, Aaron Andrews*/

/*This file is part of Dina'ki Adventures.

Dina'ki Adventures is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dina'ki Adventures is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dina'ki Adventures. If not, see <http://www.gnu.org/licenses/>.*/

/*
* Activity_0.h
*
*  Created on: Feb 12, 2014
*      Author: Aaron Andrews
*/

#ifndef MINIONCONTROLLER_MATCHING_H_
#define MINIONCONTROLLER_MATCHING_H_

#include <SFML/Audio.hpp>
#include <screen.h>
#include <activity.h>
#include "controller.h"
#include "profile.h"
#include "dictionary.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
using std::pair;
#include <iostream>
using std::cout;
using std::endl;
using std::wcout;

class minioncontroller_matching : public MinionController
{
public:
    minioncontroller_matching(std::shared_ptr<Profile> profile,
		std::shared_ptr<Interface> _interface,
                              std::shared_ptr<MasterController> masterController,
                              std::shared_ptr<Activity> activity);

    //private:
	size_t       m_score,
                 m_currentRound,
                 m_correctAnswerIndex,
                 m_currentScore,
                 m_engaged_sprite,
                 m_indexOfInstrBox,
				 m_indexOfInstrText,
				 // Get the index of the main image to be matched
				 m_mainImageIndex,
				 // Get the start of the textobjects in the screen items
				 m_textObjectsIndex,

                 m_numOfChoices;

    vector<wstring> m_wordList;
    std::shared_ptr<Activity> m_activity;
    std::shared_ptr<Interface> m_interfacePtr;

   // void clickHappened(int);
    void boxIsClicked(size_t index);
    void checkAnswer(size_t index);
    void createInitialRound();
    void createNewRound();
    void updateData();
    void showCorrectImage(size_t index);
    void endGame();
    bool gameIsOver();
    
    void mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i pos);

    wstring m_helpMessage;

private:
    void populateRound();
};

#endif /* MINIONCONTROLLER_MATCHING_H_ */
