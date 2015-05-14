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

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <cstdlib>
using std::free;

#include <SFML/Window.hpp>
#include "screen.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include "screen.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include "word.h"
#include "hud.h"
#include <memory>
#include <vector>
#include <string>
#include <stack>

class MasterController;

class MinionController
{
public:
    MinionController(std::shared_ptr<Profile> profile,
                     std::shared_ptr<Interface> interface,
                     std::shared_ptr<MasterController> masterController,
                     std::shared_ptr<Screen> screen);

    virtual ~MinionController();

    virtual void setProfile(std::shared_ptr<Profile> profile);
    
    virtual void setScreen(std::shared_ptr<Screen> screen);
    
    std::shared_ptr<Screen> getScreen();
    
    virtual void setDictionary(std::shared_ptr<Dictionary> dictionary);

    virtual void textEnteredEvent(sf::Event event);

    virtual void keyPressedEvent(sf::Event event);

    virtual void keyReleasedEvent(sf::Event event);

    virtual void mouseMovedEvent(sf::Event event, sf::Vector2i pos);

    virtual void mouseButtonPressedEvent(sf::Event event, sf::Vector2i pos);

    virtual void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i pos);

    void helpButtonPressed();

    std::vector<Word> getRandomWords(unsigned);
    
    virtual void engageItem(size_t index);
    
    virtual void selectItem(size_t index, Color col = sf::Color::Green);
    
    virtual void disengageItem(size_t index);
    
    virtual void deselectedItem(size_t index, Color col = sf::Color::Transparent);
    
    virtual void createNewProfile();

protected:
    std::shared_ptr<Screen> m_screen;
    
    std::shared_ptr<Profile> m_profile; // no cereal

    std::shared_ptr<Interface> m_interface; // no cereal

    std::shared_ptr<MasterController> m_master_controller; // no cereal
    
private:
};


class MasterController
{
public:
    MasterController();
    
    void handleEvent(sf::Event event);
    
    void closedEvent();
    
    void resizedEvent(sf::Event::SizeEvent& window_size);
    
    void lostFocusEvent();
    
    void gainedFocusEvent();
    
    void push_controller(std::shared_ptr<MinionController> new_controller);
    
    void pop_controller();
    
    //void addTimeEvent(unsigned int, sf::Time);
    
    std::shared_ptr<Profile> getProfile();
    
    std::shared_ptr<ProfileWrangler> getProfileWranlger(); 

    std::shared_ptr<Interface> getInterface();
    
    void toggleKeyboard();
    
    void toggleBackButton();
    
    void controlLoop(); 

	void closeProfileWindowIfUp();
    
private:
    std::shared_ptr<Interface> m_interface;
    
    std::shared_ptr<sf::RenderWindow> m_window;
    
    std::shared_ptr<ProfileWrangler> m_profile_wrangler;

    //std::vector<std::pair<unsigned int, sf::Time>> m_event_times;
    
    std::stack< std::shared_ptr<MinionController> > m_controllers;
    
    //sf::Clock m_clock;

    std::shared_ptr<HUD> m_hud;

    int m_engaged_sprite;

    void goBack();
    void disengageSprite(size_t sprite);
};


#endif /* end define for __CONTROLLER_H__ */
