
#include "profilecontroller.h"
#include "basicscreen.h"
#include "profile.h"
#include "screen.h"
#include "interface.h"
#include "scenecontroller.h"
#include "scene.h"
#include "map.h"
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

#include "mapcontroller.h"

#include <wctype.h>
#include <wchar.h>
#include <vector>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Audio/SoundSource.hpp>

ProfileController::ProfileController(std::shared_ptr<ProfileWrangler> wrangler, std::shared_ptr<Profile> profile,
                  std::shared_ptr<Interface> interface,
                  std::shared_ptr<MasterController> masterController,
                  std::shared_ptr<Screen> screen)
: MinionController(profile, interface, masterController, screen), m_profile_wrangler(wrangler)
{
    //gather original collection of screen items (background)
    m_screen_items = m_screen->getScreenItems();

    //create the background image
    ScreenItem t_background_image;
    t_background_image.type = IMAGE;
    t_background_image.name = L"images/greyBackground.png";
    t_background_image.size = { 800, 600 };

    //create help box and text
    ScreenItem t_help_box;
    t_help_box.type = BOX;
    t_help_box.name = L"instructionbox";
    t_help_box.size = {600, 400};
    t_help_box.position = {100, 100};
    t_help_box.color = sf::Color(0, 0, 0, 200);
    t_help_box.visible = false;
    
    ScreenItem t_help_text;
    t_help_text.type = TEXT;
    t_help_text.size = {10, 0};
    t_help_text.position = {105, 105};
    t_help_text.visible = false;

    //set text for the help box
    t_help_text.name  = L"Welcome to the profile page!\n";
    t_help_text.name += L"If you have a profile, select it and click on the GO\n";
    t_help_text.name += L"button to get to your game.\n";
    t_help_text.name += L"If you don't have a profile, create one by clicking on\n";
    t_help_text.name += L"the + button.\n";
    t_help_text.name += L"If you want to delete your profile, click on your\n";
    t_help_text.name += L"profile and press the - button.\n\n";

    t_help_text.name += L"To create a profile from the profile creation page,\n";
    t_help_text.name += L"follow these steps:\n";
    t_help_text.name += L"1. Click on the keyboard to type out your name.\n";
    t_help_text.name += L"2. Click DONE.\n";
    t_help_text.name += L"3. Click on the keyboard to type out your age.\n";
    t_help_text.name += L"4. Click DONE.\n";
    t_help_text.name += L"5. Click on your gender.\n";
    t_help_text.name += L"6. Click DONE to complete your profile!";
    
    m_screen_items.push_back(t_background_image);
    m_screen_items.push_back(t_help_box);
    m_screen_items.push_back(t_help_text); 

    //get profiles from file
    std::vector<std::shared_ptr<Profile>> t_profiles = m_profile_wrangler->loadProfiles();
    
    //top position
    float top_pos = 40;
    
    // create and place screen items for boxes and texts
    for(unsigned int box = 0; box < 4; ++box)
    {
        ScreenItem t_screen_item;
        
        t_screen_item.type = BOX;
        
        if(t_profiles.size() > box)
            t_screen_item.name = L"Name: " + (t_profiles[box]->getName() + L"\nAGE: " + std::to_wstring(t_profiles[box]->getAge()));
        else
            t_screen_item.name = L"";
        
        t_screen_item.position = {100,top_pos};
        t_screen_item.size     = {600,100};
        t_screen_item.visible  = true;
        t_screen_item.hover    = true;
        
        t_screen_item.color    = m_box_color;
        
        t_screen_item.behavior = HOVER_ENGAGE | MOUSE_UP_SELECT;
        
        ScreenItem t_screen_item_text;
        t_screen_item_text.type = TEXT;
        
        if(t_profiles.size() > box)
            t_screen_item_text.name = t_screen_item.name;
        else
            t_screen_item_text.name = L"";
        
        t_screen_item_text.position = {120,top_pos};
        t_screen_item_text.size     = {20,0};
        t_screen_item_text.visible  = true;
        t_screen_item_text.hover    = true;
        t_screen_item_text.color =  Color::White;
                                      
        m_screen_items.push_back(t_screen_item);
        m_screen_items.push_back(t_screen_item_text);
        
        top_pos += 140;
    }
    
    //create plus, minus and go buttons
    ScreenItem plusButton;
    plusButton.type = BUTTON;
    plusButton.name = L"+";
    plusButton.size = {60,60};
    plusButton.color = Color(140,140,140,255);
    plusButton.position = {20,top_pos -120};
    plusButton.behavior = MOUSE_DOWN_ENGAGE | MOUSE_UP_ENGAGE;
    plusButton.sound = L"audio/other/click2.wav";
    m_screen_items.push_back(plusButton);
    
    ScreenItem minusButton;
    minusButton.type = BUTTON;
    minusButton.name = L"-";
    minusButton.size = {60,60};
    minusButton.color = Color(140,140,140,255);
    minusButton.position = {20,top_pos -200};
    minusButton.behavior = MOUSE_DOWN_ENGAGE | MOUSE_UP_ENGAGE;
    minusButton.sound = L"audio/other/click2.wav";
    m_screen_items.push_back(minusButton);
    
    ScreenItem goButton;
    goButton.type = BUTTON;
    goButton.name = L"Go";
    goButton.size = {60,60};
    goButton.color = Color(140,140,140,255);
    goButton.position = {720,top_pos -120};
    goButton.behavior = MOUSE_DOWN_ENGAGE | MOUSE_UP_ENGAGE;
    goButton.sound = L"audio/other/click2.wav";
    m_screen_items.push_back(goButton);
    
    ScreenItem newProfileBox;
    newProfileBox.type = BOX;
    newProfileBox.name = L"newProfileBox";
    newProfileBox.size = {600,200};
    newProfileBox.color = m_box_color;
    newProfileBox.position = {100,100};
    newProfileBox.visible = false;
    m_screen_items.push_back(newProfileBox);
    
    ScreenItem newProfileNameTag;
    newProfileNameTag.type = TEXT;
    newProfileNameTag.name = L"Name: ";
    newProfileNameTag.size = {20,20};
    newProfileNameTag.color = sf::Color::White;
    //newProfileNameTag.behavior = MOUSE_UP_ENGAGE;
    newProfileNameTag.position = {110,100};
    newProfileNameTag.visible = false;
    m_screen_items.push_back(newProfileNameTag);
    m_name_tag = m_screen_items.size()-1;
    
    ScreenItem newProfileName;
    newProfileName.type = TEXT;
    newProfileName.name = L"";
    newProfileName.size = {20,20};
    newProfileName.color = sf::Color::White;
    newProfileName.position = {220,100};
    newProfileName.visible = false;
    m_screen_items.push_back(newProfileName);
    m_name_field = m_screen_items.size()-1;
    
    ScreenItem newProfileAgeTag;
    newProfileAgeTag.type = TEXT;
    newProfileAgeTag.name = L"Age: ";
    newProfileAgeTag.size = {20,20};
    newProfileAgeTag.color = sf::Color::White;
    //newProfileAgeTag.behavior = MOUSE_UP_ENGAGE;
    newProfileAgeTag.position = {110,140};
    newProfileAgeTag.visible = false;
    m_screen_items.push_back(newProfileAgeTag);
    m_age_tag = m_screen_items.size()-1;
    
    ScreenItem newProfileAge;
    newProfileAge.type = TEXT;
    newProfileAge.name = L"";
    newProfileAge.size = {20,20};
    newProfileAge.color = sf::Color::White;
    newProfileAge.position = {200,140};
    newProfileAge.visible = false;
    m_screen_items.push_back(newProfileAge);
    m_age_field = m_screen_items.size()-1;
    
    ScreenItem maleButton;
    maleButton.type = BUTTON;
    maleButton.name = L"Male";
    maleButton.size = {200,60};
    maleButton.position = {150,210};
    maleButton.color = Color(150,150,150,255);
    maleButton.visible = false;
    maleButton.sound = L"audio/other/click2.wav";
    maleButton.behavior = MOUSE_DOWN_ENGAGE;
    m_screen_items.push_back(maleButton);
    
    ScreenItem femaleButton;
    femaleButton.type = BUTTON;
    femaleButton.name = L"Female";
    femaleButton.size = {200,60};
    femaleButton.position = {450,210};
    femaleButton.color = Color(150,150,150,255);
    femaleButton.visible = false;
    femaleButton.sound = L"audio/other/click2.wav";
    femaleButton.behavior = MOUSE_DOWN_ENGAGE;
    m_screen_items.push_back(femaleButton);
    
    // Change the model
    m_screen->setScreenItems(m_screen_items);
    // Update the interface
    m_interface->update(0, m_screen->getScreenItems().size(), m_screen->getScreenItems());
}

void ProfileController::setFieldHighlight()
{
    if((size_t)m_current_edit == m_name_field)
    {
        disengageItem(m_age_tag);
        engageItem(m_name_tag);
    }
    else if((size_t)m_current_edit == m_age_field)
    {
        disengageItem(m_name_tag);
        engageItem(m_age_tag);
    }
    else
    {
        disengageItem(m_age_tag);
        disengageItem(m_age_tag);
        
    }
}

void ProfileController::togleNewProfileBox()
{
    if(m_engaged_sprite >-1)
    {
        disengageItem((size_t)m_engaged_sprite);
        m_engaged_sprite = -1;
    }
    if(m_selected_sprite >-1)
    {
        deselectedItem((size_t)m_selected_sprite);
        m_selected_sprite = -1;
    }
    
    m_screen_items = m_screen->getScreenItems();
    
    m_show_new_profile = !m_show_new_profile;
    
    for(size_t profileBoxes  = 3; profileBoxes < 21; ++profileBoxes)
    {
        m_screen_items[profileBoxes].visible = !m_screen_items[profileBoxes].visible;
    }
    
    m_screen_items[m_name_field].name = L"";
    m_screen_items[m_age_field].name = L"";
    
    m_master_controller->toggleKeyboard();
    
    m_screen->setScreenItems(m_screen_items);
    m_interface->update(3,21,m_screen->getScreenItems());
    
    setFieldHighlight();
}

void ProfileController::updateProfiles()
{
    m_screen_items = m_screen->getScreenItems();
    
    std::vector<std::shared_ptr<Profile>> t_profiles( m_profile_wrangler->loadProfiles());
    
    for(unsigned int box = 0; box < 4; ++box)
    {
        if(t_profiles.size() > box + m_top_profile)
        {
            m_screen_items[2*box+3].name = L"Name: " + (t_profiles[box+ m_top_profile]->getName() + L"\nAGE: " + std::to_wstring(t_profiles[box]->getAge()));
            m_screen_items[2*box+4].name = m_screen_items[2*box+3].name;
            m_screen_items[2*box+4].visible = true;
        }
        else
        {
            m_screen_items[2*box+3].name = L"";
            m_screen_items[2*box+4].name = L"";
            m_screen_items[2*box+4].visible = false;
        }
    }
    
    m_screen->setScreenItems(m_screen_items);
    m_interface->update(0, m_screen_items.size(), m_screen->getScreenItems());
}

void ProfileController::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    //Check if we have moused over anything interesting
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    
    // Adjust hit sprite so that it falls to boxes for profiles
    if(hit_sprite == 4 || hit_sprite == 6 || hit_sprite == 8 || hit_sprite == 10)
    {
        --hit_sprite;
    }
    
    // If we hit anything besides the backgorund
    if(hit_sprite > -1)
    {
        ScreenItem n_screen_item = m_screen->getScreenItem((size_t)hit_sprite);
        
        if(m_engaged_sprite != hit_sprite)
        {
            if(m_engaged_sprite > 0)
            {
                ScreenItem screen_item = m_screen->getScreenItem((size_t)m_engaged_sprite);
                screen_item.hover = false;
                
                if(screen_item.behavior & HOVER_ENGAGE)
                {
                    disengageItem((size_t)m_engaged_sprite);
                    m_engaged_sprite = -1;
                }
            }
            
            if(n_screen_item.behavior & HOVER_ENGAGE)
            {
                m_engaged_sprite = hit_sprite;
                engageItem((size_t)hit_sprite);
            }

        }
        
        if (m_selected_sprite != hit_sprite )
        {
            if(m_selected_sprite > 0)
            {
                ScreenItem screen_item = m_screen->getScreenItem((size_t)m_selected_sprite);
                screen_item.hover = false;
                
                if(screen_item.behavior & HOVER_SELECT)
                {
                    deselectedItem((size_t)m_selected_sprite, m_box_color);
                    m_selected_sprite =-1;
                }
            }
            
            if(n_screen_item.behavior & HOVER_SELECT)
            {
                m_selected_sprite = hit_sprite;
                selectItem((size_t)hit_sprite, m_box_color);
            }

        }
        
    }
}

void ProfileController::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    
    if(hit_sprite == 4 || hit_sprite == 6 || hit_sprite == 8 || hit_sprite == 10)
    {
        --hit_sprite;
    }
    
	if (m_selected_sprite != hit_sprite)
	{
		m_master_controller->closeProfileWindowIfUp();
	}

    if( hit_sprite > -1)
    {
        ScreenItem screen_item;
        screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];
        
        if (m_engaged_sprite != hit_sprite && m_engaged_sprite > 0)
        {
            disengageItem((size_t)m_engaged_sprite);
        }
        
        if (m_selected_sprite != hit_sprite && m_selected_sprite > 0
            && screen_item.name != L"-"
            && screen_item.name != L"Go")
        {
            deselectedItem((size_t)m_selected_sprite, m_box_color);
            m_selected_sprite = -1;
            m_profile_wrangler->setCurrentProfile(-1);
        }
        
        if (screen_item.behavior & MOUSE_UP_ENGAGE)
        {
            m_interface->playScreenSound((size_t)hit_sprite);
            
            if(screen_item.type == BUTTON)
            {
                m_engaged_sprite = -1;
                disengageItem((size_t)hit_sprite);
            }
            else
            {
                m_engaged_sprite = hit_sprite;
                engageItem((size_t)m_engaged_sprite);
            }
            
            if(screen_item.name == L"-" && m_selected_sprite > 0)
            {
                deselectedItem((size_t)m_selected_sprite,m_box_color);
                
                m_profile_wrangler->removeProfile();
                m_selected_sprite = -1;
                m_profile_wrangler->setCurrentProfile(-1);
                
                updateProfiles();
            }
            else if(screen_item.name == L"+")
            {
                if(m_current_edit != -1)
                    disengageItem((size_t)m_current_edit);
                m_current_edit = (int)m_name_field;
                togleNewProfileBox();
            }
            else if(screen_item.name == L"Go" && m_selected_sprite > 0)
            {
                
                deselectedItem((size_t)m_selected_sprite, m_box_color);
                m_profile_wrangler->setCurrentProfile((m_selected_sprite-3)/2 + (int)m_top_profile);
                //m_master_controller->getProfile();
                m_selected_sprite = -1;
                //std::cout << m_profile_wrangler->getCurrentProfile()->getAge() << std::endl;
                
                std::shared_ptr<Map> game_map = make_shared<Map>("WorldMap");
                game_map->load();
                
                std::shared_ptr<MapController> map_controller =
                        make_shared<MapController>(m_master_controller->getProfile(),
                                                   m_master_controller->getInterface(),
                                                   m_master_controller, game_map);
                
                m_master_controller->push_controller(map_controller);
            }
        }
        
        if (screen_item.behavior & MOUSE_UP_SELECT)
        {
            if(hit_sprite > 2 && hit_sprite < 10
               && m_profile_wrangler->getProfiles().size() > (m_top_profile + (size_t)(hit_sprite-3) / 2 ))
            {
                m_selected_sprite = hit_sprite;
                selectItem((size_t)hit_sprite, sf::Color::White);
                
                m_profile_wrangler->setCurrentProfile((int)m_top_profile + (hit_sprite-3) / 2 );
            }
            else
            {
                m_selected_sprite = -1;
            }
        }

    }
}

void ProfileController::mouseButtonPressedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    
    if(hit_sprite == 4 || hit_sprite == 6 || hit_sprite == 8 || hit_sprite == 10)
    {
        --hit_sprite;
    }
    
    if( hit_sprite > -1 )
    {
        ScreenItem screen_item = m_screen->getScreenItem((size_t)hit_sprite);
        
        if (m_engaged_sprite != hit_sprite && m_engaged_sprite > 0)
        {
            
            if(m_screen->getScreenItem((size_t)m_engaged_sprite).behavior & MOUSE_DOWN_ENGAGE)
                disengageItem((size_t)m_engaged_sprite);
        }
        
        if (m_selected_sprite != hit_sprite && m_selected_sprite > 0)
        {
            
            if(m_screen->getScreenItem((size_t)m_selected_sprite).behavior & MOUSE_DOWN_SELECT)
                deselectedItem((size_t)m_selected_sprite, m_box_color);
        }
        
        
        if( screen_item.behavior & MOUSE_DOWN_SELECT )
        {
            m_selected_sprite = hit_sprite;
            selectItem((size_t)m_selected_sprite, sf::Color::White);
        }
        
        if( screen_item.behavior & MOUSE_DOWN_ENGAGE )
        {
            m_interface->playScreenSound((size_t)hit_sprite);
            
            m_engaged_sprite = hit_sprite;
            engageItem((size_t)m_engaged_sprite);
        }
    }
}

void ProfileController::textEnteredEvent(sf::Event event)
{
    
    //std::cout << (wchar_t) event.text.unicode << std::endl;
    if(m_current_edit != -1)
    {
        ScreenItem new_text = m_screen->getScreenItem((size_t)m_current_edit);
        
        if(wchar_t(event.text.unicode) == 8 && new_text.name.size() > 0)
        {
            new_text.name = new_text.name.substr(0, new_text.name.size()-1);
        }
        else if(wchar_t(event.text.unicode) == 8)
        {
            if((size_t)m_current_edit == m_name_field)
            {
                //disengageItem((size_t)m_current_edit-2);
                m_current_edit = -1;
                togleNewProfileBox();
                setFieldHighlight();
                return;
            }
            else if((size_t)m_current_edit == m_age_field)
            {
                //disengageItem((size_t)m_current_edit-2);
                m_current_edit = (int)m_name_field;
                setFieldHighlight();
                return;
            }
        }
        else if(m_interface->isCharSupported(wchar_t(event.text.unicode)))
        {
            if((size_t)m_current_edit == m_name_field && new_text.name.size() > 22)
            {
                new_text.name = new_text.name.substr(0, new_text.name.size()-1) + (wchar_t(event.text.unicode));
            }
            else if((size_t)m_current_edit == m_name_field)
            {
                new_text.name += (wchar_t(event.text.unicode));
            }
            else if((size_t)m_current_edit == m_age_field && iswdigit(wchar_t(event.text.unicode)))
            {
                if(new_text.name.size() > 23)
                    new_text.name = new_text.name.substr(0, new_text.name.size()-1) + (wchar_t(event.text.unicode));
                else
                    new_text.name += (wchar_t(event.text.unicode));
            }
        }
        else if((wchar_t(event.text.unicode)) == '\n' || (wchar_t(event.text.unicode)) == 9 || (wchar_t(event.text.unicode)) == 10 || (wchar_t(event.text.unicode)) == '\r')
        {
            if((size_t)m_current_edit == m_name_field)
            {
                m_current_edit = (int)m_age_field;
                setFieldHighlight();
                return;
            }
            else if((size_t)m_current_edit == m_age_field)
            {
                if(m_screen->getScreenItem((size_t)m_name_field).name.size() > 0 && m_screen->getScreenItem((size_t)m_age_field).name.size() > 0 && m_engaged_sprite > -1)
                {
                    Profile::gender t_gen = Profile::female;
                    if(m_screen->getScreenItem((size_t)m_engaged_sprite).name == L"Male")
                        t_gen = Profile::male;
                        
                    m_profile_wrangler->addProfile(std::make_shared<Profile>(
                                                                             m_screen->getScreenItem((size_t)m_name_field).name,
                                                                             std::stol(m_screen->getScreenItem((size_t)m_age_field).name),
                                                                             t_gen
                                                                             )
                                                   );
                    //disengageItem((size_t)m_current_edit-2);
                    m_current_edit = -1;
                    togleNewProfileBox();
                    updateProfiles();
                }
                setFieldHighlight();
                return;
            }
        }
        

        //engageItem((size_t)m_current_edit-1);
        m_screen->setScreenItem((size_t)m_current_edit, new_text);
        m_interface->update((size_t)m_current_edit, m_screen->getScreenItem((size_t)m_current_edit));
    }
    
    setFieldHighlight();
}

void ProfileController::createNewProfile()
{
    //disengageItem((size_t)m_current_edit);
    m_current_edit = (int)m_name_field;
    togleNewProfileBox();
}

void ProfileController::keyPressedEvent(sf::Event event)
{
    if(event.key.code == sf::Keyboard::Up && m_top_profile > 0)
    {
       --m_top_profile;
        updateProfiles();
    }
    else if(event.key.code == sf::Keyboard::Down)
    {
        if(m_profile_wrangler->getProfiles().size() > 4 + m_top_profile)
            ++m_top_profile;
        
        updateProfiles();
    }
    /*if(event.key.code == sf::Keyboard::P)
    {
        //std::cout << "plus " << std::endl;
        std::shared_ptr<Profile> n_profile(std::make_shared<Profile>());
        m_profile_wrangler->addProfile(n_profile);
        m_profile_wrangler->saveProfiles();
    }
    else if(event.key.code == sf::Keyboard::M)
    {
        //std::cout << "minus " << std::endl;
        
        if(m_profile_wrangler->getProfiles().size() > 0)
        {
            m_profile_wrangler->removeProfile(m_profile_wrangler->getProfiles().back());
            m_profile_wrangler->saveProfiles();
        }
    }*/
}

