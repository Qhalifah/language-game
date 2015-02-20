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
