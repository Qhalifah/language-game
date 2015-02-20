#include "mapcontroller.h"
#include "screen.h"
#include "interface.h"
#include "scenecontroller.h"
#include "scene.h"
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

bool MapController::locked(unsigned index)
{
    bool locked = false;
    for( auto badge : m_map->scenePrereqs().at(index) )
    {
        cout << "almost" << endl;
        cout << "sz:" << badge.size() << endl;
        cout << "nm:" << badge << endl;
        //if(badge.size() && !m_profile->lookupBadge(badge))
        string hi = "none";
        if(badge != hi && !m_profile->lookupBadge(badge))
        {
            locked = true;
            break;
        }
    }
    cout << "locked?" << locked << endl;
    return locked;
}

void MapController::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

    if (m_engaged_sprite != hit_sprite && hit_sprite > -1)
    {// Then we are over a sprite, and it is not the currently engaged one.
        ScreenItem screen_item;

        if (m_engaged_sprite > -1)
        {// Then there is a sprite which is currently engaged.
            // Disengage the currently engaged sprite
            disengageItem((size_t)m_engaged_sprite);
            m_engaged_sprite = -1;
        }

        screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if (screen_item.behavior & HOVER_ENGAGE)
        {// Then the screen item is engageable on hover
            // If it is a scene location, find out if it is unlocked
            if( m_map->getScene((size_t)hit_sprite) != "" )
            {
                if( locked((size_t)hit_sprite) )
                    screen_item.color = sf::Color::Red;
                else
                    screen_item.color = sf::Color::Green;
            }

            screen_item.hover = true;
            screen_item.engaged = true;
            m_screen->setScreenItem((size_t)hit_sprite, screen_item);
            m_interface->update((size_t)hit_sprite, screen_item);
            m_engaged_sprite = hit_sprite;
        }

        if (screen_item.behavior & HOVER_SELECT)
        {
            screen_item.selected = !screen_item.selected;
            if( screen_item.selected )
                screen_item.color = sf::Color::Green;
            m_screen->setScreenItem((size_t)hit_sprite, screen_item);
            m_interface->update((size_t)hit_sprite, screen_item);
        }
    }
}

void MapController::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

    if( hit_sprite > -1 ) // It is a valid sprite
    {
        ScreenItem screen_item;

        screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if( m_engaged_sprite != hit_sprite )
        {// Then we are over a sprite, and it is not the currently engaged one.
            if( m_engaged_sprite > -1 )
            {// Then there is a sprite which is currently engaged.
                // Disengage the currently engaged sprite
                disengageItem((size_t)m_engaged_sprite);
                m_engaged_sprite = -1;
            }

            if (screen_item.behavior & MOUSE_UP_ENGAGE)
            {// Then the screen item is engageable on hover
                engageItem((size_t)hit_sprite);
                m_engaged_sprite = hit_sprite;
            }
        }

        if (screen_item.behavior & MOUSE_UP_SELECT)
        {
            screen_item.selected = !screen_item.selected;
            if( screen_item.selected )
                screen_item.color = sf::Color::Green;
            m_interface->update((size_t)hit_sprite, screen_item);
        }

        string scene = m_map->getScene((size_t)hit_sprite);
        if( scene != "" )
        {
            // If scene isn't locked, then load it
            if( !locked((size_t)hit_sprite) )
            {
                m_interface->playScreenSound((size_t)hit_sprite);
                loadScene(scene);
            }
        }
    }
}

void MapController::disengageSprite(int sprite, ScreenItem& screen_item)
{
    screen_item.engaged = false;
    m_interface->update((size_t)m_engaged_sprite, screen_item);
    m_engaged_sprite = -1;
}

void MapController::loadScene(string scene_name)
{
    std::shared_ptr<Scene> new_scene(new Scene(scene_name));
    new_scene->load();
    std::shared_ptr<SceneController>
    scene_controller(new SceneController(m_master_controller->getProfile(),
                                         m_master_controller->getInterface(),
                                         m_master_controller,
                                         new_scene));


    m_master_controller->push_controller(scene_controller);
}
