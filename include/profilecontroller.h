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

#ifndef __PROFILE_CONTROLLER_H__
#define __PROFILE_CONTROLLER_H__

#include "screen.h"
#include "controller.h"
#include "profilewrangler.h"
#include "interface.h"
#include "dictionary.h"
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
#include <string>
#include <stack>

class ProfileController : public MinionController
{
public:
    ProfileController(std::shared_ptr<ProfileWrangler> wrangler, std::shared_ptr<Profile> profile,
		std::shared_ptr<Interface> _interface,
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

	std::string m_DocumentsPath;        // Used to get the user's Document file path
};

#endif /* end define for __PROFILE_CONTROLLER_H__ */
