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
