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

#include "matchingcontroller.h"
#include "word.h"
#include <iostream>
using std::wcout;
using std::endl;

minioncontroller_matching::minioncontroller_matching(std::shared_ptr<Profile> profile,
                                                     std::shared_ptr<Interface> interface,
                                                     std::shared_ptr<MasterController> masterController,
                                                     std::shared_ptr<Activity> activity)
                                                     : MinionController(profile, interface, masterController, activity), 
                                                     m_activity(activity), m_interfacePtr(interface)
{

    m_helpMessage  = L"In this game, I'll give you a bunch of words and then a\n";
    m_helpMessage += L"picture of one of them.All you have to do is click on the\n";
    m_helpMessage += L"word that matches the picture! Isn't that easy ? Just keep\n";
    m_helpMessage += L"doing it until you win!\n\n";
    m_helpMessage += L"Don't feel too bad if you mess up.You can replay these\n";
    m_helpMessage += L"games as often as you need.Have fun!";

    m_activity->setHelpMessage( m_helpMessage );

    m_grade = 0;
    m_engaged_sprite = 0;
    m_currentRound = 0;
    m_indexOfInstrBox = 1;
	m_indexOfInstrText = 2;
	// Get the index of the main image to be matched
	m_mainImageIndex = 3;
	// Get the start of the textobjects in the screen items
	m_textObjectsIndex = m_activity->getScreenItems().size() - m_activity->getRounds();
    m_numOfChoices = m_activity->getChoices();
	createInitialRound();

	ScreenItem t_screen_item = m_activity->getScreenItem(m_indexOfInstrBox);

};


void minioncontroller_matching::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    if (((size_t)hit_sprite >= m_textObjectsIndex && (size_t)hit_sprite < m_activity->getScreenItems().size()) || 
		hit_sprite == m_mainImageIndex)
    {
        ScreenItem m_screen_item;
		
        // If hit sprite is not the currently engaged sprite
		if (m_engaged_sprite != (size_t)hit_sprite && m_engaged_sprite < m_activity->getScreenItems().size() - 1)
        {
            m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
            m_screen_item.engaged = false;
            m_interface->update(m_engaged_sprite, m_screen_item);
        }
        
        m_engaged_sprite = (size_t)hit_sprite;
        m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
        
        m_screen_item.engaged = true;
        m_interface->update(m_engaged_sprite, m_screen_item);
	}
	// If no sprite to be the engaged sprite
	else if (hit_sprite > -1 && (size_t)hit_sprite < m_activity->getScreenItems().size() - 1 && m_engaged_sprite > 0)
    {
        ScreenItem m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
            
        m_screen_item.engaged = false;
        m_interface->update(m_engaged_sprite, m_screen_item);
        m_engaged_sprite = (size_t)hit_sprite;
    }
}

void minioncontroller_matching::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i pos)
{
    if (gameIsOver())
    {
        m_master_controller->pop_controller();
    }
    else
    {
		int hit_sprite = m_interfacePtr->getScreenHit(pos.x, pos.y);
        
		if (hit_sprite > m_textObjectsIndex || hit_sprite == m_mainImageIndex)
			m_interface->playScreenSound((size_t)hit_sprite);

        // if index is one of the clickable text boxes
		if ((size_t)hit_sprite >= m_textObjectsIndex && 
			hit_sprite < (int)(m_activity->getScreenItems().size()))
        {
			checkAnswer((size_t)hit_sprite);
        }
    }
}

void minioncontroller_matching::setGrade(size_t newGrade)
{
	m_grade = newGrade;
}

size_t  minioncontroller_matching::getGrade()
{
    return m_grade;
}


void minioncontroller_matching::checkAnswer(size_t index)
{
	std::cout << "clicked box's index: " << index << ", correct answer index: " << m_correctAnswerIndex + m_textObjectsIndex << std::endl;
    //check index of clicked box against stored index of correct answer
	if (index == m_correctAnswerIndex + m_textObjectsIndex)
    {

		cout << "Answer correct." << endl;
        ScreenItem t_screenitem;

		for (auto ii = m_mainImageIndex + 1; ii < m_textObjectsIndex; ++ii)
        {
			cout << "Checking if index " << ii << " is invisble." << endl;

			// Make the first invisble reward item visible
            if (!m_screen->getScreenItems()[ii].visible)
            {
				cout << "Answer correct, making reward item visible." << endl;

                t_screenitem = m_screen->getScreenItems()[ii];
                t_screenitem.visible = true;
                m_screen->setScreenItem(ii, t_screenitem);
                m_interface->update(ii, t_screenitem);
                break;
            }
        }
        m_grade += 20;
        createNewRound();
    }
    else
    {
        createNewRound();
    }
}

void minioncontroller_matching::populateRound()
{
    ++m_currentRound;

    if (gameIsOver())
    {
        endGame();
    }
    else
    {
        vector<ScreenItem> t_screenitems = m_screen->getScreenItems();
        vector<Word> t_wordlist;

        t_wordlist = getRandomWords(m_activity->getChoices());

		for (size_t ii = 0; ii < m_activity->getChoices(); ++ii)
        {
			t_screenitems[ii + m_textObjectsIndex].name = t_wordlist[ii].getDinName();
            
            if(m_profile->getGender() == Profile::female)
				t_screenitems[ii + m_textObjectsIndex].sound = t_wordlist[ii].getGirlAudioLocation();
            else
				t_screenitems[ii + m_textObjectsIndex].sound = t_wordlist[ii].getBoyAudioLocation();
        }

        //randomly choose a selected word to be the correct answer
        srand(unsigned(time(NULL)));
        m_correctAnswerIndex = ((size_t)rand() % m_numOfChoices);
        t_screenitems[m_mainImageIndex].name = t_wordlist[m_correctAnswerIndex].getImageLocation();
        if(m_profile->getGender() == Profile::female)
			t_screenitems[m_mainImageIndex].sound = t_wordlist[m_correctAnswerIndex].getGirlAudioLocation();
        else
			t_screenitems[m_mainImageIndex].sound = t_wordlist[m_correctAnswerIndex].getBoyAudioLocation();
        m_screen->setScreenItems(t_screenitems);
    }
}

void minioncontroller_matching::createInitialRound()
{
	vector<ScreenItem> t_screen_items = m_activity->getScreenItems();

	int xRatio = m_activity->getSize().x / 800;
	int yRatio = m_activity->getSize().y / 600;

	ScreenItem t_screen_item;

	// Create the main image
	t_screen_item.type = IMAGE;
	t_screen_item.name = L"None";
	t_screen_item.size = Vec2(200 * xRatio, 200 * yRatio);
	t_screen_item.visible = true;
	t_screen_item.behavior = 24; //     MOUSE_UP_SELECT = 8 and HOVER_ENGAGE = 16,
	t_screen_item.position = Vec2(550 * xRatio, 100 * yRatio);

	t_screen_items.push_back(t_screen_item);

	// Create the reward image items
	t_screen_item.type = IMAGE;
	t_screen_item.name = m_activity->getRewardImage();
	t_screen_item.size = Vec2(50 * xRatio, 50 * yRatio);
	t_screen_item.visible = false;
	t_screen_item.behavior = 0; //     None
	
	for (int ii = 0; ii < m_activity->getRounds(); ++ii)
	{
		t_screen_item.position.x = m_screen->getScreenItems()[0].size.x * .4
			* (1.0 + (rand() % 100) / 200.0);
		t_screen_item.position.y = m_screen->getScreenItems()[0].size.y * .6
			* (1.0 + (rand() % 100) / 200.0);
		t_screen_items.push_back(t_screen_item);
	}

	// Create the Text items
	t_screen_item.type = TEXT;
	t_screen_item.name = L"e";
	t_screen_item.size = Vec2(20, 0);
	t_screen_item.visible = true;
	t_screen_item.color = Color::Black;
	t_screen_item.hover = true;
	t_screen_item.behavior = 24; //     MOUSE_UP_SELECT = 8 and HOVER_ENGAGE = 16,
	
	m_textObjectsIndex = t_screen_items.size();
	int spacer = m_screen->getScreenItems()[0].size.y / m_activity->getChoices();

	for (int ii = 0; ii < m_activity->getChoices(); ++ii)
	{
		t_screen_item.position = Vec2(100, spacer + ii * 90);
		t_screen_items.push_back(t_screen_item);
	}

	m_screen->setScreenItems(t_screen_items);

    populateRound();
}


void minioncontroller_matching::createNewRound()
{
    populateRound();

	// Update the text objects
    m_interface->update(m_textObjectsIndex, 
		m_activity->getScreenItems().size(), 
		m_screen->getScreenItems());

	// Update the main image
	m_interface->update(m_mainImageIndex, m_screen->getScreenItems()[m_mainImageIndex]);
}


bool minioncontroller_matching::gameIsOver()
{
    return (m_currentRound > m_activity->getRounds());
}

void minioncontroller_matching::endGame()
{
    vector<ScreenItem> t_screenitems = m_screen->getScreenItems();

    // change background screen to black
    t_screenitems[0].name = L"images/activity_endscreen.png";
    m_interface->update(0, t_screenitems[0]);

    // set box with player score to visible
    t_screenitems[m_indexOfInstrBox].visible = true;
    m_interface->update(m_indexOfInstrBox, t_screenitems[m_indexOfInstrBox]);
    // set text with player score to visible
    setGrade(m_grade);

	if (m_grade >= m_activity->getMaxScore())
	{
		m_profile->addBadgePiece(m_activity->getBadgePiece());
	}

    t_screenitems[m_indexOfInstrText].name = L"Your score is " + std::to_wstring(getGrade());
	t_screenitems[m_indexOfInstrText].name += m_activity->getBadgePiece().m_badge_name;
    t_screenitems[m_indexOfInstrText].size.x = 30;
    t_screenitems[m_indexOfInstrText].visible = true;
    m_interface->update(m_indexOfInstrText, t_screenitems[m_indexOfInstrText]);

    // set all other screen items to invisible
    for (auto ii = m_mainImageIndex; ii < t_screenitems.size(); ++ii)
    {
        t_screenitems[ii].visible = false;
        m_interface->update(ii, t_screenitems[ii]);
    }
    m_screen->setScreenItems(t_screenitems);
}
