#ifndef __PROFILE_CONTROLLER_H__
#define __PROFILE_CONTROLLER_H__

#include "screen.h"
#include "controller.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include <memory>
#include <vector>
#include <string>
#include <stack>

class ProfileController : public MinionController
{
public:
    ProfileController(std::shared_ptr<ProfileWrangler> wrangler, std::shared_ptr<Profile> profile,
                  std::shared_ptr<Interface> interface,
                  std::shared_ptr<MasterController> masterController,
                      std::shared_ptr<Screen> screen);
    
    void mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void mouseButtonPressedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc);
    void keyPressedEvent(sf::Event event);
    void textEnteredEvent(sf::Event event);
    void createNewProfile();
    void setFieldHighlight();
    
private:
    void updateProfiles();
    void togleNewProfileBox();
    bool isNumber(wchar_t wch);
    
    std::shared_ptr<ProfileWrangler> m_profile_wrangler;
    
    std::vector<ScreenItem> m_screen_items;
    
    Color m_box_color = Color(0,0,200,240);
    
    size_t m_top_profile = 0;
    int m_engaged_sprite = 0;
    int m_selected_sprite = 0;
    
    size_t m_name_tag = 0;
    size_t m_age_tag = 0; 
    size_t m_name_field = 0;
    size_t m_age_field = 0;
    
    int m_current_edit = -1; 
    bool m_show_new_profile = false; 
};

#endif /* end define for __PROFILE_CONTROLLER_H__ */
