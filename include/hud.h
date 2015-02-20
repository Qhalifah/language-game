/*
 * hud.h
 *
 *  Created on: Mar 20, 2014
 *      Author: shaun
 */

#ifndef HUD_H_
#define HUD_H_

#include <SFML/Window.hpp>
#include "screen.h"
#include "interface.h"
#include "profile.h"
#include <string>
using std::string;
#include <SFML/Graphics.hpp>
using sf::Vector2f;
#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

class Interface;

// Affectionately called HUD for Heads Up Display.
// Interface buttons which are Screen independent and always visible.
class HUD
{
public:
    HUD();

    const vector<ScreenItem>& getItems() const;
    void toggleKeys(shared_ptr<Interface>);
    void toggleProfile(shared_ptr<Interface> interface, std::shared_ptr<Profile>);
    void toggleBackButton(shared_ptr<Interface>);
    void engage(size_t index);
    void disengage(size_t index);
    const float width() const;
    const float height() const;
    const size_t firstKey() const;
    
    bool isProfileUp();
    bool isKeyboardUp();
    bool isBackButtonUp();
    
    void rotateBadges(int, std::shared_ptr<Profile> );
    void updateProfile(std::shared_ptr<Interface>,  std::shared_ptr<Profile>);
    
    void handleHit(std::shared_ptr<Interface> interface, int hitsptrie);
    
private:
    bool m_profile_up = false;
    bool m_back_button_up = false;
    bool m_keyboard_up = false;
    
    const float m_width  = 100;
    const float m_height = 10;
    
    size_t m_firstKey;
    size_t m_badge_index;
    vector<ScreenItem> m_buttons;    // List of buttons in HUD
    
    size_t m_profile_index;
    size_t m_name_index;
    size_t m_age_index;
    size_t m_back_button_index;
    size_t m_badge_image_index;
    
    int m_last_hover = -1;
};


#endif /* HUD_H_ */
