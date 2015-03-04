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

#ifndef __MAP_CONTROLLER_H__
#define __MAP_CONTROLLER_H__

#include <cstdlib>
using std::free;

#include <SFML/Window.hpp>
#include "screen.h"
#include "map.h"
#include "controller.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include <memory>
#include <vector>
#include <string>
#include <stack>

class MapController : public MinionController
{
public:
    MapController(std::shared_ptr<Profile> profile,
                  std::shared_ptr<Interface> interface,
                  std::shared_ptr<MasterController> masterController,
                  std::shared_ptr<Map> map)
                  : MinionController(profile, interface, masterController, map), m_engaged_sprite(0), m_map(map)
{
        m_map->setHelpMessage();
        unordered_map<unsigned, string> t_strVec;
        t_strVec.insert(pair<unsigned, string>(3, "RiverScene"));
        t_strVec.insert(pair<unsigned, string>(4, "VillageScene"));
        //m_map->setScenes(t_strVec);
};

    void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc);

    void mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc);

private:
    int m_engaged_sprite;
    shared_ptr<Map> m_map;

    bool locked(unsigned index);
    void disengageSprite(int sprite, ScreenItem& screen_item);
    void loadScene(string scene_name);
};

#endif /* end define for __MAP_CONTROLLER_H__ */
