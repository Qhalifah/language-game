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

#include "scenecontroller.h"
#include "paircontroller.h"
#include "matchingcontroller.h"
#include "screen.h"
#include "interface.h"
#include "scene.h"
#include "dictionary.h"
#include "word.h"

#include <iostream>
using std::cout;
using std::endl;

void SceneController::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);



    if( hit_sprite > -1 && m_engaged_sprite != hit_sprite )
    {// Then sprite is valid and not already engaged.
        ScreenItem screen_item;

        if( m_engaged_sprite > -1 )
        {// There is a sprite which is currently engaged.
            // Disengage currently engaged sprite.
            screen_item = m_screen->getScreenItems()[(size_t)m_engaged_sprite];
            screen_item.engaged = false;
            screen_item.hover = false;
            m_screen->setScreenItem((size_t)m_engaged_sprite, screen_item);
            m_interface->update((size_t)m_engaged_sprite, screen_item);
            m_engaged_sprite = -1;
        }

        screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if( screen_item.behavior & HOVER_ENGAGE )
        {// Then sprite is engageable.
            screen_item.engaged = true;
            screen_item.hover   = true;
            m_screen->setScreenItem((size_t)hit_sprite, screen_item);
            m_interface->update((size_t)hit_sprite, screen_item);

            // Currently engaged sprite is now the one we are over.
            m_engaged_sprite = hit_sprite;
        }

        if( screen_item.behavior & HOVER_SELECT )
        {
            //TODO: implement this
        }
    }
}


void SceneController::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

    if( hit_sprite > -1 && (int)m_screen->getScreenItems().size() > hit_sprite)
    {
        ScreenItem m_screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if( m_screen_item.behavior & MOUSE_UP_SELECT )
        {
            // Toggle selected status
            m_screen_item.selected = !m_screen_item.selected;

            if (m_screen_item.selected)
            {
                m_interface->playScreenSound((size_t)hit_sprite);

                // Set color of selection box
                m_screen_item.color = sf::Color::Green;

                // Add to set of selected items
                m_myset.insert((size_t)hit_sprite);

                // Update sprite
                m_screen->setScreenItem(unsigned(hit_sprite), m_screen_item);
                m_interface->update((size_t)hit_sprite, m_screen_item);

                // If the current set matches one of the sets of requirements,
                // Launch the appropriate activity
                if ( m_scene->m_requirements.count(m_myset) )
                {
                    std::shared_ptr<MinionController> act_controller;

                    std::shared_ptr<Activity> new_activity(new Activity(m_scene->m_activities[(size_t)m_scene->m_requirements[m_myset]].second));
                    cout << "hi world" << endl;
                    new_activity->load();
                    cout << "bye world" << endl;
                    act_controller = getGameType( m_scene->m_activities[(size_t)m_scene->m_requirements[m_myset]].first , new_activity);

                    auto screenItems = m_screen->getScreenItems();

                    for(auto ii = 0u; ii < screenItems.size(); ++ii)
                    {
                        screenItems[ii].selected = false;
                        screenItems[ii].engaged = false;
                        m_screen->setScreenItem(ii, screenItems[ii]);
                    }

                    m_master_controller->push_controller(act_controller);
                    m_myset.clear();
                }
            }
            else // Not selected
            {
                // Remove from set of selected items
                m_myset.erase((size_t)hit_sprite);

                // Update sprite
                m_screen->setScreenItem((size_t)hit_sprite, m_screen_item);
                m_interface->update((size_t)hit_sprite, m_screen_item);
            }
        } // End MOUSE_UP_SELECT

        if( m_screen_item.behavior & MOUSE_UP_ENGAGE )
        {
            //TODO: implement this in case user wants this behavior
        }
    }
}

void SceneController::mouseButtonPressedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

    if( hit_sprite > 1 ) // Ensure that it is part of the screen
    {
        ScreenItem m_screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if( m_screen_item.behavior & MOUSE_DOWN_SELECT )
        {
            //TODO: implement this in case user wants this behavior
        }

        if( m_screen_item.behavior & MOUSE_DOWN_ENGAGE )
        {
            //TODO: implement this in case user wants this behavior
        }
    }
}

std::shared_ptr<MinionController> SceneController::getGameType(GameType gameType, std::shared_ptr<Activity> activityPtr)
{
    switch (gameType){
        case PAIR:
        {

            return make_shared<PairController>(m_master_controller->getProfile(),
                                               m_master_controller->getInterface(),
                                               m_master_controller,
                                               activityPtr);
        }
        case MATCHING:
        {

            return make_shared<minioncontroller_matching>(m_master_controller->getProfile(),
                                                          m_master_controller->getInterface(),
                                                          m_master_controller,
                                                          activityPtr);
        }
        default:
        {
            return nullptr;
        }
    }
}
