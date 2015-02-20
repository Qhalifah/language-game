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
                    std::shared_ptr<Interface> interface,
                    std::shared_ptr<MasterController> masterController,
                    std::shared_ptr<Scene> scene)
                    :MinionController(profile, interface, masterController, scene),
                    m_engaged_sprite(0), m_scene(scene) {

        //MiniGame t_miniGame = (std::make_pair(PAIR, "Pair"));
        //m_scene->m_activities.push_back(t_miniGame);
        //t_miniGame = (std::make_pair(MATCHING, "Matching"));
        //m_scene->m_activities.push_back(t_miniGame);

        //std::set<int> tempSet0 = { 3, 4 };
        //std::set<int> tempSet1 = { 3, 5 };
        //m_scene->m_requirements.insert(m_scene->m_requirements.begin(), std::pair<set<int>, int>(tempSet0, 0));
        //m_scene->m_requirements.insert(m_scene->m_requirements.begin(), std::pair<set<int>, int>(tempSet1, 1));
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
