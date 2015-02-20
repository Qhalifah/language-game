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

class minioncontroller_matching : public MinionController
{
public:
    minioncontroller_matching(std::shared_ptr<Profile> profile,
                              std::shared_ptr<Interface> interface,
                              std::shared_ptr<MasterController> masterController,
                              std::shared_ptr<Activity> activity);

    //private:
    size_t       m_grade,
                 m_currentRound,
                 m_correctAnswerIndex,
                 m_currentScore,
                 m_engaged_sprite,
                 m_numOfScrItems,
                 m_indexOfInstrBox,
                 m_indexOfInstrText,
                 m_indexOfPicture,
                 m_numOfChoices;

    vector<wstring> m_wordList;
    std::shared_ptr<Activity> m_activity;
    std::shared_ptr<Interface> m_interfacePtr;

    void setGrade(size_t newGrade);
    size_t  getGrade();
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
