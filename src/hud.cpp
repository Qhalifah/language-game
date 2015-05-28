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

// hud.cpp
// Shaun Bond
// 3/21/2014
//
// Implementation of the HUD class


#include "hud.h"
#include "profile.h"

#include <set>
#include "badge.h"
#include <iterator>

HUD::HUD() : m_buttons(6, ScreenItem())
{
    float buttonWidth  = m_width / (m_buttons.size() + 1);
    float buttonHeight = m_height * .5;
    float spacer = buttonWidth / (m_buttons.size() + 1);

    for( auto ii = 0u; ii < m_buttons.size(); ++ii )
    {
        m_buttons[ii].type = BUTTON;
        m_buttons[ii].color = Color(150, 150, 150);
        m_buttons[ii].size = Vec2(buttonWidth, buttonHeight);
        m_buttons[ii].position = Vec2(spacer+(buttonWidth+spacer)*ii,
                                      buttonHeight*.5);
        m_buttons[ii].sound = L"audio/other/click2.wav";
        m_buttons[ii].volume = 100;
    }

    m_buttons[0].name = L"New";
    m_buttons[1].name = L"Load";
    m_buttons[2].name = L"Save";
    m_buttons[3].name = L"Profile";
    m_buttons[4].name = L"Exit";
    m_buttons[5].name = L"?";

    ScreenItem temp;

    // Add back button
    temp.type = IMAGE;
    temp.name = L"images/back.png";
    temp.size = {m_height, m_height};
    temp.position = {spacer, m_height + spacer};
    temp.sound = L"audio/other/pageturn.wav";
    temp.visible = false; 
    m_buttons.push_back(temp);
    m_back_button_index = m_buttons.size() -1;

    // Add images for keyboard
    // Start with container for keys
    temp.type = BOX;
    temp.name = L"keybox";
    temp.sound = L"none";
    temp.color = Color(0, 0, 0, 150);
    temp.visible = false;
    m_buttons.push_back(temp);

    m_firstKey = m_buttons.size();

    // Create virtual keys
    temp.type = BUTTON;
    temp.size = {m_height * .5f, m_height * .5f};
    temp.sound = L"audio/other/click2.wav";
    temp.color = Color(150, 150, 150, 255);
    temp.visible = false;

    // Set position and size of container
    m_buttons[m_firstKey-1].size = {11*(temp.size.x+1)+1, 4*(temp.size.y+1)+1};
    m_buttons[m_firstKey-1].position = {(m_width-m_buttons[m_firstKey-1].size.x)*.5f,
                                        (float)(5.5*m_height)};

    // Set position of virtual keys
    for(auto ii = 0u; ii < 10; ++ii)
    {
        temp.position = Vec2(m_buttons[m_firstKey-1].position.x+1+.5*temp.size.x+(temp.size.x+1)*ii,
                             m_buttons[m_firstKey-1].position.y + 1);
        m_buttons.push_back(temp);
    }
    for(auto ii = 0u; ii < 10; ++ii)
    {
        temp.position = Vec2(m_buttons[m_firstKey-1].position.x+1+(temp.size.x+1)*ii,
                             m_buttons[m_firstKey-1].position.y + 2 + m_height*.5);
        m_buttons.push_back(temp);
    }
    for(auto ii = 0u; ii < 10; ++ii)
    {
        temp.position = Vec2(m_buttons[m_firstKey-1].position.x+1+(temp.size.x+1)*ii,
                             m_buttons[m_firstKey-1].position.y + 3 + m_height*1.0);
        m_buttons.push_back(temp);
    }
    for(auto ii = 0u; ii < 10; ++ii)
    {
        temp.position = Vec2(m_buttons[m_firstKey-1].position.x+1 + (temp.size.x+1)*ii,
                             m_buttons[m_firstKey-1].position.y + 4 + m_height*1.5);
        m_buttons.push_back(temp);
    }

    // Label virtual keys
    m_buttons[m_firstKey].name = L"0";
    m_buttons[m_firstKey+1].name = L"1";
    m_buttons[m_firstKey+2].name = L"2";
    m_buttons[m_firstKey+3].name = L"3";
    m_buttons[m_firstKey+4].name = L"4";
    m_buttons[m_firstKey+5].name = L"5";
    m_buttons[m_firstKey+6].name = L"6";
    m_buttons[m_firstKey+7].name = L"7";
    m_buttons[m_firstKey+8].name = L"8";
    m_buttons[m_firstKey+9].name = L"9";
    m_buttons[m_firstKey+10].name = L"A";
    m_buttons[m_firstKey+11].name  = L"B";
    m_buttons[m_firstKey+12].name  = L"C";
    m_buttons[m_firstKey+13].name  = L"D";
    m_buttons[m_firstKey+14].name  = L"E";
    m_buttons[m_firstKey+15].name  = L"F";
    m_buttons[m_firstKey+16].name  = L"G";
    m_buttons[m_firstKey+17].name  = L"H";
    m_buttons[m_firstKey+18].name  = L"I";
    m_buttons[m_firstKey+19].name  = L"Space";
    m_buttons[m_firstKey+19].size  = {2*temp.size.x+1, temp.size.y};
    m_buttons[m_firstKey+20].name = L"J";
    m_buttons[m_firstKey+21].name = L"K";
    m_buttons[m_firstKey+22].name = L"L";
    m_buttons[m_firstKey+23].name = L"M";
    m_buttons[m_firstKey+24].name = L"N";
    m_buttons[m_firstKey+25].name = L"O";
    m_buttons[m_firstKey+26].name = L"P";
    m_buttons[m_firstKey+27].name = L"Q";
    m_buttons[m_firstKey+28].name = L"R";
    m_buttons[m_firstKey+29].name = L"Back";
    m_buttons[m_firstKey+29].size  = {2*temp.size.x+1, temp.size.y};
    m_buttons[m_firstKey+30].name = L"S";
    m_buttons[m_firstKey+31].name = L"T";
    m_buttons[m_firstKey+32].name = L"U";
    m_buttons[m_firstKey+33].name = L"V";
    m_buttons[m_firstKey+34].name = L"W";
    m_buttons[m_firstKey+35].name = L"X";
    m_buttons[m_firstKey+36].name = L"Y";
    m_buttons[m_firstKey+37].name = L"Z";
    m_buttons[m_firstKey+38].name = L"\u0141";
    m_buttons[m_firstKey+39].name = L"Done";
    m_buttons[m_firstKey+39].size  = {2*temp.size.x+1, temp.size.y};
    
    // Profile information overlay starts here
    m_profile_index = m_buttons.size(); 
    ScreenItem t_screen_item;
    
    Vec2 text_size = {2.5,2.5};
    Color text_color = Color(255,255,255,255);
    
    t_screen_item.type = BOX;
    t_screen_item.name = L"profile";
    t_screen_item.sound = L"none";
    t_screen_item.size = {80, 40};
    t_screen_item.position = {10,10};
    t_screen_item.color = Color(0, 0, 0, 150);
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    t_screen_item.type = TEXT;
    t_screen_item.name = L"Name: ";
    t_screen_item.sound = L"none";
    t_screen_item.size = text_size;
    t_screen_item.position = {10,10};
    t_screen_item.color = text_color;
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    t_screen_item.type = TEXT;
    t_screen_item.name = L"Age: ";
    t_screen_item.sound = L"none";
    t_screen_item.size = text_size;
    t_screen_item.position = {10,20};
    t_screen_item.color = text_color;
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    // Place holder for profile name
    t_screen_item.type = TEXT;
    t_screen_item.name = L"";
    t_screen_item.sound = L"none";
    t_screen_item.size = text_size;
    t_screen_item.position = {23,10};
    t_screen_item.color = text_color;
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    m_name_index = m_buttons.size() -1;
    
    // Place holder for user age
    t_screen_item.type = TEXT;
    t_screen_item.name = L"";
    t_screen_item.sound = L"none";
    t_screen_item.size = text_size;
    t_screen_item.color = text_color;
    t_screen_item.position = {21,20};
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    m_age_index = m_buttons.size() -1;
    
	// Create placeholders for the badges
    t_screen_item.type = IMAGE;
    t_screen_item.name = L"none";
    t_screen_item.hover_text = L"none";
    t_screen_item.hover = false;
    t_screen_item.size = {20,20};
    t_screen_item.position = {15,27};
    t_screen_item.sound = L"none";
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    m_badge_image_index = m_buttons.size() -1;
    m_badge_index = 0;
    
    t_screen_item.type = IMAGE;
    t_screen_item.name = L"none";
    t_screen_item.hover_text = L"none";
    t_screen_item.hover = false;
    t_screen_item.size = {20,20};
    t_screen_item.position = {40,27};
    t_screen_item.sound = L"none";
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
    
    t_screen_item.type = IMAGE;
    t_screen_item.name = L"none";
    t_screen_item.hover_text = L"none";
    t_screen_item.hover = false;
    t_screen_item.size = {20,20};
    t_screen_item.position = {65,27};
    t_screen_item.sound = L"none";
    t_screen_item.visible = false;
    m_buttons.push_back(t_screen_item);
}

void HUD::handleHit(std::shared_ptr<Interface> interface, int hitsprite)
{
    if(m_last_hover != hitsprite)
    {
        if(m_last_hover != -1)
        {
            m_buttons[(size_t)m_last_hover].hover = false;
            interface->updateHud((size_t)m_last_hover, m_buttons[(size_t)m_last_hover]);
            m_last_hover = -1;
        }
        
        if(hitsprite != -1 && (size_t)hitsprite < m_badge_image_index + 3 && (size_t)hitsprite >= m_badge_image_index)
        {
            m_buttons[(size_t)hitsprite].hover = true;
            m_last_hover = hitsprite;
            interface->updateHud((size_t)m_last_hover, m_buttons[(size_t)m_last_hover]);
        }
    }
}

bool HUD::isProfileUp()
{
    return m_profile_up;
}

bool HUD::isKeyboardUp()
{
    return m_keyboard_up;
}

bool HUD::isBackButtonUp()
{
    return m_back_button_up;
}

void HUD::rotateBadges(int rot, std::shared_ptr<Profile> profile)
{
    if(profile->getBadgeItemCount()> m_badge_index + 3 && rot > 0)
        m_badge_index++;
    else if(m_badge_index > 0 && rot < 0)
        m_badge_index--;
}

void HUD::updateProfile(shared_ptr<Interface> interface, std::shared_ptr<Profile> t_profile)
{
    std::vector<Badge> t_badges = t_profile->getBadges();
	cout << "inside updateProfile" << endl;

    size_t curr_display_badge = m_badge_image_index;
    for(size_t pos_badge = m_badge_index;
        pos_badge < t_badges.size() && curr_display_badge < m_badge_image_index + 3;
        ++pos_badge, ++curr_display_badge)
    {
		cout << "inside updateProfile for loop, looking at ";
		string s((const char*)&t_badges[pos_badge].m_name[0], sizeof(wchar_t) / sizeof(char)*t_badges[pos_badge].m_name.size());
		for (int ii = 0; ii < s.size(); ++ii)
			cout << s;
		cout << endl;
		cout << "t_badges.size(): " << t_badges.size() << endl;
        if(t_badges[pos_badge].isComplete())
        {
            m_buttons[curr_display_badge].name = t_badges[pos_badge].m_image;
            m_buttons[curr_display_badge].hover_text = t_badges[pos_badge].m_name;
        }
        else
        for(size_t pos_piece = 0;
            pos_piece < t_badges[pos_badge].m_pieces.size() && curr_display_badge < m_badge_image_index+3;
            ++pos_piece, ++curr_display_badge)
            {
                m_buttons[curr_display_badge].name = t_badges[pos_badge].m_pieces[pos_piece].m_image;
                m_buttons[curr_display_badge].hover_text = t_badges[pos_badge].m_name + L" " + std::to_wstring(t_badges[pos_badge].m_pieces[pos_piece].m_id);
            }
    }
    /*size_t badge = m_badge_index;
    
    for(; badge < m_badge_index+3 && t_profile->getBadgeItemCount() > badge; ++ badge)
    {

            size_t t_badge = badge;
            for(size_t piece = 0; piece < t_badges[t_badge].m_pieces.size() && badge < m_badge_index+3; ++piece)
            {
                m_buttons[badge + m_badge_image_index - m_badge_index].name = t_badges[t_badge].m_pieces[piece].m_image;
                m_buttons[badge + m_badge_image_index - m_badge_index].hover_text = t_badges[t_badge].m_name + L" " + std::to_wstring(t_badges[t_badge].m_pieces[piece].m_id);
                badge++; 
            }
        
            if(badge < t_badges.size())
            {
                m_buttons[badge + m_badge_image_index - m_badge_index].name = t_badges[badge].m_image;
                m_buttons[badge + m_badge_image_index - m_badge_index].hover_text = t_badges[badge].m_name;
            }
    }*/

	// This will display up to three badges
    for(;curr_display_badge <m_badge_image_index+3; ++curr_display_badge )
    {
        m_buttons[curr_display_badge].name = L"none";
        m_buttons[curr_display_badge].hover_text = L"none";
    }
    
	std::cout << "m_buttons.size(): " << m_buttons.size() << std::endl;
	std::cout << "m_badge_image_index: " << m_badge_image_index << std::endl;

	//updates the badges within the profile box
    //interface->updateHud(m_badge_image_index, m_buttons.size(), m_buttons);
	interface->updateHud(m_badge_image_index, m_buttons.size(), m_buttons);
}

//
void HUD::toggleProfile(shared_ptr<Interface> interface, std::shared_ptr<Profile> t_profile)
{
    //std::cout << name.size() << age.size() << std::endl;
    // Set name and age of profile to display
    if(t_profile != nullptr) {
        m_buttons[m_name_index].name = t_profile->getName();
        m_buttons[m_age_index].name = std::to_wstring(t_profile->getAge());
    }
    
    // Toggle visibility of all profile overlay objects
    for(size_t ii = m_profile_index; ii < m_buttons.size(); ++ii)
    {
        // TODO: improve synchronization with m_profile_up
        m_buttons[(size_t)ii].visible = !m_buttons[(size_t)ii].visible;
    }
    
	if (t_profile != nullptr) {
		cout << "t_profile != nullptr" << endl;
        updateProfile(interface, t_profile);
    }
    else {
		cout << "m_profile_index: " << m_profile_index << endl;
    }

	interface->updateHud(m_profile_index, m_buttons.size(), m_buttons);
    m_profile_up = !m_profile_up;
}

void HUD::toggleKeys(shared_ptr<Interface> interface)
{
    // m_firstKey - 1 to account for container
    for(auto ii = unsigned(m_firstKey-1); ii < m_profile_index; ++ii)
    {
        m_buttons[ii].visible = !m_buttons[ii].visible;
    }

    m_keyboard_up = !m_keyboard_up;
    interface->updateHud(m_firstKey-1, m_profile_index, m_buttons);
}

void HUD::toggleBackButton(shared_ptr<Interface>  interface)
{
    m_buttons[m_back_button_index].visible = !m_buttons[m_back_button_index].visible;
    
    m_back_button_up = !m_back_button_up;
    interface->updateHud(m_back_button_index, m_buttons[m_back_button_index]);
}

void HUD::engage(size_t index)
{
    if(index < m_buttons.size() )
        m_buttons[index].engaged = true;
}

void HUD::disengage(size_t index)
{
    if( index < m_buttons.size() )
        m_buttons[index].engaged = false;
}

const vector<ScreenItem>& HUD::getItems() const
{
    return  m_buttons;
}

const float HUD::width() const
{
    return m_width;
}

const float HUD::height() const
{
    return m_height;
}

const size_t HUD::firstKey() const
{
    return m_firstKey;
}
