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

#ifndef __SCENE_CONTROLLER_H__
#define __SCENE_CONTROLLER_H__

#include <cstdlib>
using std::free;

#include <SFML/Window.hpp>
#include "screen.h"
#include "activity.h"
#include "scene.h"
#include "controller.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include <memory>
#include <vector>
#include <string>
#include <stack>

class SceneController : public MinionController
{
public:
    SceneController(std::shared_ptr<Profile> profile,
		std::shared_ptr<Interface> _interface,
                    std::shared_ptr<MasterController> masterController,
                    std::shared_ptr<Scene> scene)
					:MinionController(profile, _interface, masterController, scene),
                    m_engaged_sprite(0), m_scene(scene) {

        m_scene->setHelpMessage();

        Badge t_badge = m_scene->badge();
                        
        if(t_badge.m_name != L"")
            m_profile->addBadge(t_badge);
    };

    void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void mouseButtonPressedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc);

private:
    int m_engaged_sprite;
    std::map<std::set<int>, string> activityMap;
    std::set<unsigned> m_myset;
    shared_ptr<Scene> m_scene;
    std::shared_ptr<MinionController> getGameType(GameType, std::shared_ptr<Activity>);
};

#endif /* end define for __SCENE_CONTROLLER_H__ */
