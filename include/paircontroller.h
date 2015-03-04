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

#ifndef __PAIR_CONTROLLER_H__
#define __PAIR_CONTROLLER_H__

#include <cstdlib>
using std::free;

#include <SFML/Window.hpp>
#include "screen.h"
#include "activity.h"
#include "controller.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include <memory>
//#include <vector>
#include <set>
using std::set;
#include <string>
#include <stack>

class PairController : public MinionController
{
public:
    PairController(std::shared_ptr<Profile> profile,
                   std::shared_ptr<Interface> interface,
                   std::shared_ptr<MasterController> masterController,
                   std::shared_ptr<Activity> screen);
    
    void mouseButtonPressedEvent(sf::Event event, sf::Vector2i pos);
    
    void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc);
    
    void mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc);
    
    void randomizeWords(unsigned int index, unsigned int length);

private:
    int m_engaged_sprite;
    wstring m_helpMessage;

    unsigned int m_count;

    unsigned int m_answered;
    unsigned int m_tries;

    set<set<unsigned>> m_pairs;

    set<unsigned> m_selected;

    shared_ptr<Activity> m_activity;

    bool gameIsOver();

    void endGame();
};

#endif /* end define for __PAIR_CONTROLLER_H__ */
