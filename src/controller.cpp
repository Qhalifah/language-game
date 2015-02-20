#include "screen.h"
#include "controller.h"
#include "interface.h"
#include "map.h"
#include "dictionary.h"
#include "profile.h"
#include "screen.h"
#include <string>
#include <vector> 
#include <fstream>

MinionController::MinionController(std::shared_ptr<Profile> profile,
                 std::shared_ptr<Interface> interface,
                 std::shared_ptr<MasterController> masterController,
                 std::shared_ptr<Screen> screen)
: m_screen(screen),
  m_profile(profile),
  m_interface(interface),
  m_master_controller(masterController) {};

MinionController::~MinionController() {};


void MinionController::createNewProfile()
{
}

void MinionController::setProfile(std::shared_ptr<Profile> profile)
{
    m_profile = profile;
};

void MinionController::setScreen(std::shared_ptr<Screen> screen)
{
    m_screen = screen;
};

std::shared_ptr<Screen> MinionController::getScreen()
{
    return m_screen;
}

void MinionController::setDictionary(std::shared_ptr<Dictionary> dictionary)
{
    m_screen->setDictionary(dictionary);
}

void MinionController::textEnteredEvent(sf::Event event) {};

void MinionController::keyPressedEvent(sf::Event event) {};

void MinionController::keyReleasedEvent(sf::Event event) {};

void MinionController::mouseMovedEvent(sf::Event event, sf::Vector2i) {};

void MinionController::mouseButtonPressedEvent(sf::Event event, sf::Vector2i) {};

void MinionController::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i){};

std::vector<Word> MinionController::getRandomWords(unsigned numberOfWords)
{
    Word t_word;
    vector<Word> listToSend;
    std::set<wstring> t_wordset;
    listToSend.reserve(numberOfWords);
    shared_ptr<Dictionary> t_dictionary = m_screen->getDictionary();
    
    t_word = t_dictionary->getRandomWord();
    listToSend.push_back(t_word);

    while (listToSend.size() < numberOfWords)
    {
        t_word = t_dictionary->getRandomWord();
        for (auto ii = 0u; ii < listToSend.size(); ++ii)
        {
            if (listToSend[ii].getDinName() == t_word.getDinName())
                break;
            if (ii == listToSend.size()-1)
                listToSend.push_back(t_word);
        }
    }
    return listToSend;
}

void MinionController::helpButtonPressed()
{
    if (m_screen->getScreenItems().size() > 2)
    {
        ScreenItem t_boxitem = m_screen->getScreenItems()[1];
        ScreenItem t_textitem = m_screen->getScreenItems()[2];
        t_boxitem.visible = !t_boxitem.visible;
        t_textitem.visible = !t_textitem.visible;
        m_screen->setScreenItem(1, t_boxitem);
        m_screen->setScreenItem(2, t_textitem);
        m_interface->update(1, t_boxitem);
        m_interface->update(2, t_textitem);
    }
}

void MinionController::engageItem(size_t index)
{
    ScreenItem screen_item = m_screen->getScreenItems()[index];
    screen_item.hover = true;
    screen_item.engaged = true;
    m_screen->setScreenItem(index, screen_item);
    m_interface->update(index, screen_item);
}

void MinionController::selectItem(size_t index, Color col)
{
    ScreenItem screen_item = m_screen->getScreenItems()[index];
    screen_item.selected = true;
    screen_item.color = col;
    m_screen->setScreenItem(index, screen_item);
    m_interface->update(index, screen_item);
}

void MinionController::disengageItem(size_t index)
{
    ScreenItem screen_item = m_screen->getScreenItems()[index];
    screen_item.hover = false;
    screen_item.engaged = false;

    m_screen->setScreenItem(index, screen_item);
    m_interface->update(index, screen_item);
}

void MinionController::deselectedItem(size_t index, Color col)
{
    ScreenItem screen_item = m_screen->getScreenItems()[index];
    screen_item.selected = false;
    screen_item.color = col;
    m_screen->setScreenItem(index, screen_item);
    m_interface->update(index, screen_item);
    
}

MasterController::MasterController()
: m_interface(new Interface()), m_window(m_interface->getWindow()),
  m_profile_wrangler(new ProfileWrangler()), m_controllers(),
  m_hud(new HUD()), m_engaged_sprite(-1)
{
    //m_profile_wrangler->loadProfiles();
    m_interface->changeHUD(m_hud);

    /*
    // TODO: decide who pushes minion controller onto stack
    MinionController temp_controller(m_profile_wrangler.getCurrentProfile(),
                                     m_interface,
                                     std::shared_ptr<MasterController>(this));
    
    std::ifstream is(start_controller);
    
    if(is.good())
    {
        cereal::BinaryInputArchive archive(is);
        
        archive( temp_controller );
    }
    
    push_controller(temp_controller);*/
}

void MasterController::handleEvent(sf::Event event)
{
    //Logic will go here that will check if event is related to menu!
    if(event.type == sf::Event::Closed )
    {
        closedEvent();
    }
    else if(event.type == sf::Event::Resized)
    {
        m_interface->resize();
        resizedEvent(event.size);
    }
    else if(event.type == sf::Event::LostFocus)
    {
        //I stop the main BGM, you should stop any extra you need!
        m_interface->stopBGM(0);
        
        lostFocusEvent();
    }
    else if(event.type == sf::Event::GainedFocus)
    {
        m_interface->playBGM(0);
        gainedFocusEvent();
    }
    else if(event.type == sf::Event::TextEntered)
    {
        m_controllers.top()->textEnteredEvent(event);
    }
    else if(event.type == sf::Event::KeyPressed)
    {
        if(m_hud->isProfileUp() && event.key.code == sf::Keyboard::Escape)
        {
            m_hud->toggleProfile(m_interface, getProfile());
        }
        else if(event.key.code == sf::Keyboard::Escape)
        {
            goBack();
        }
        
        m_controllers.top()->keyPressedEvent(event);
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::F1)
        {
            m_hud->toggleKeys(m_interface);
        }
        else if(event.key.code == sf::Keyboard::F2)
        {
            if(getProfile() != nullptr)
                m_hud->toggleProfile(m_interface, getProfile());
        }
        else if(event.key.code == sf::Keyboard::F3)
        {
            if(getProfile() != nullptr)
            {
                Badge t_badge;
                t_badge.m_name = L"Test Badge One";
                t_badge.m_image = L"images/map.png";
                getProfile()->addBadge(t_badge);
                
                t_badge.m_name = L"Test Badge Two";
                t_badge.m_image = L"images/rubberboots.png";
                t_badge.m_total_pieces = 3;
                getProfile()->addBadge(t_badge);
                Piece t_piece;
                t_piece.m_id = 1;
                t_piece.m_image = L"images/fish.png";
                t_piece.m_badge_name = t_badge.m_name;
                getProfile()->addBadgePiece(t_piece);
                t_piece.m_id = 0;
                t_piece.m_image = L"images/rod.png";
                t_piece.m_badge_name = t_badge.m_name;
                getProfile()->addBadgePiece(t_piece);
                
                                t_badge.m_total_pieces = 0;
                
                t_badge.m_name = L"Test Badge Three";
                t_badge.m_image = L"images/map.png";
                getProfile()->addBadge(t_badge);
                
                //t_badge.m_name = L"Test Badge Four";
                //t_badge.m_image = L"images/boat.png";
                //getProfile()->addBadge(t_badge);
            }
        }
        else if(event.key.code == sf::Keyboard::F4)
        {
            if(getProfile() != nullptr)
            {
                
            }
        }
        
        if(m_hud->isProfileUp() && event.key.code == sf::Keyboard::Right)
        {
            m_hud->rotateBadges(1, getProfile());
            m_hud->updateProfile(m_interface, getProfile());
        }
        else if(m_hud->isProfileUp() && event.key.code == sf::Keyboard::Left)
        {
            m_hud->rotateBadges(-1, getProfile());
            m_hud->updateProfile(m_interface, getProfile());
        }
        else
        {
            m_controllers.top()->keyReleasedEvent(event);
        }

    }
    else if(event.type == sf::Event::MouseMoved)
    {
        Vector2i mouse_loc = sf::Mouse::getPosition(*m_window);
        int hit_sprite = m_interface->getHudHit(mouse_loc.x, mouse_loc.y);

        m_hud->handleHit(m_interface,hit_sprite);
        
        if(hit_sprite == -1)
        {
            if(m_engaged_sprite > -1)
            {
                disengageSprite((size_t)m_engaged_sprite);
            }

            m_controllers.top()->mouseMovedEvent
                                    (event, sf::Mouse::getPosition(*m_window));
        }
        
        else if(m_engaged_sprite != hit_sprite)
        {
            if(m_engaged_sprite > -1)
            {
                disengageSprite((size_t)m_engaged_sprite);
            }
            if(m_hud->getItems()[(size_t)hit_sprite].name == L"images/back.png")
            {
                m_hud->engage((size_t)hit_sprite);
                m_interface->updateHud((size_t)hit_sprite, m_hud->getItems()[(size_t)hit_sprite]);
                m_engaged_sprite = hit_sprite;
            }
        }
        

    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        Vector2i mouse_loc = sf::Mouse::getPosition(*m_window);

        int hit_sprite = m_interface->getHudHit(mouse_loc.x, mouse_loc.y);

        if(hit_sprite > -1 && m_engaged_sprite != hit_sprite)
        {
            if(m_hud->getItems()[(size_t)hit_sprite].name != L"images/back.png")
            {
                m_hud->engage((size_t)hit_sprite);
                m_interface->playHudSound((size_t)hit_sprite);
                m_interface->updateHud((size_t)hit_sprite, m_hud->getItems()[(size_t)hit_sprite]);
                m_engaged_sprite = hit_sprite;
            }
        }
        else if(hit_sprite == -1)
        {
            m_controllers.top()->mouseButtonPressedEvent
                                (event, sf::Mouse::getPosition(*m_window));
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        Vector2i mouse_loc = sf::Mouse::getPosition(*m_window);

        int hit_sprite = m_interface->getHudHit(mouse_loc.x, mouse_loc.y);

        if(hit_sprite > -1 && m_engaged_sprite == hit_sprite)
        {
            m_interface->playHudSound((size_t)hit_sprite);
            disengageSprite((size_t)hit_sprite);
            if((size_t)hit_sprite < m_hud->firstKey())
            {
                if(m_hud->getItems()[(size_t)hit_sprite].name == L"Exit")
                {
                    closedEvent();
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"images/back.png")
                {
                    goBack();
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"?")
                {
                    m_controllers.top()->helpButtonPressed();
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"Save")
                {
                    m_profile_wrangler->saveProfiles();
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"Load" )
                {
                    while(m_controllers.size() > 1)
                    {
                        pop_controller();
                    }
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"Profile")
                {
                    //std::cout << (getProfile()->getAge()) << std::endl;
                    if(getProfile() != nullptr)
                        m_hud->toggleProfile(m_interface, getProfile());
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"New")
                {
                    while(m_controllers.size() > 1)
                    {
                        pop_controller();
                    }
                    
                    m_controllers.top()->createNewProfile();
                }
            }
            else
            { // It is one of the virtual keys
                sf::Event t_event;
                //sf::Event n_event;
                t_event.type = sf::Event::TextEntered;
                //n_event.type = sf::Event::KeyPressed;
                if(m_hud->getItems()[(size_t)hit_sprite].name == L"Space")
                {
                    t_event.text.unicode = L' ';
                    //n_event.key.code = sf::Keyboard::Space;
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"Back")
                {
                    t_event.text.unicode = 8;
                    //n_event.key.code = sf::Keyboard::BackSpace;
                }
                else if(m_hud->getItems()[(size_t)hit_sprite].name == L"Done")
                {
                    t_event.text.unicode = L'\n';
                    // n_event.key.code = sf::Keyboard::Return;
                }
                else
                { // It is a normal character to pass on
                    t_event.text.unicode = (uint32_t)m_hud->getItems()[(size_t)hit_sprite].name[0];
                    /*if((m_hud->getButtons()[(size_t)hit_sprite].name[0] >= 'A' && m_hud->getButtons()[(size_t)hit_sprite].name[0] <= 'Z'))
                        n_event.key.code = sf::Keyboard::Key(m_hud->getButtons()[(size_t)hit_sprite].name[0] - 'A');
                    if((m_hud->getButtons()[(size_t)hit_sprite].name[0] >= 'a' && m_hud->getButtons()[(size_t)hit_sprite].name[0] <= 's'))
                        n_event.key.code = sf::Keyboard::Key(m_hud->getButtons()[(size_t)hit_sprite].name[0] - 'a');*/
                }
                handleEvent(t_event);
                //handleEvent(n_event);
            }
        }
        else if(hit_sprite == -1)
        {
            m_controllers.top()->mouseButtonReleasedEvent(event, mouse_loc);
        }
    }
}

void MasterController::disengageSprite(size_t sprite)
{
    m_hud->disengage(sprite);
    m_interface->updateHud(sprite, m_hud->getItems()[sprite]);
    m_engaged_sprite = -1;
}

void MasterController::goBack()
{
    if(m_controllers.size() == 1)
        closedEvent();

    pop_controller();
}

void MasterController::closedEvent()
{
    sf::sleep(sf::seconds(.75));
    m_profile_wrangler->saveProfiles();
    exit(0);
}

void MasterController::resizedEvent(sf::Event::SizeEvent& window_size)
{
    //Tell interface that window size has changed and they need to update this...
}

void MasterController::lostFocusEvent()
{
    m_profile_wrangler->saveProfiles();
}

void MasterController::gainedFocusEvent()
{
}

void MasterController::push_controller(std::shared_ptr<MinionController> temp_controller)
{
    if(m_controllers.size() == 1)
    {
        toggleBackButton();
    }
    
    m_controllers.push(temp_controller);
    m_interface->changeMode(m_controllers.top()->getScreen());
}

void MasterController::pop_controller()
{
    if(m_controllers.size() == 2)
    {
        toggleBackButton();
        m_controllers.pop();
    }
    else if(m_controllers.size()>1)
    {
        m_controllers.pop();
    }
    
    m_interface->changeMode(m_controllers.top()->getScreen());
}

//void MasterController::addTimeEvent(unsigned int index, sf::Time time)
//{
//    m_event_times.push_back(std::make_pair(index, m_clock.getElapsedTime() + time));
//}

std::shared_ptr<Profile> MasterController::getProfile()
{
    return m_profile_wrangler->getCurrentProfile(); 
}

std::shared_ptr<ProfileWrangler> MasterController::getProfileWranlger()
{
    return m_profile_wrangler; 
}

std::shared_ptr<Interface> MasterController::getInterface()
{
    return m_interface; 
}

void MasterController::toggleKeyboard()
{
    m_hud->toggleKeys(m_interface); 
}

void MasterController::toggleBackButton()
{
    m_hud->toggleBackButton(m_interface);
}

void MasterController::controlLoop()
{
    sf::Event local_event;
    
    while(m_window->waitEvent(local_event))
    {
        handleEvent(local_event);
    }

    /*while(true)
    {
        if(m_window->pollEvent(local_event))
        {
            handleEvent(local_event);
        }

        auto tevent = m_event_times.begin();

        while( tevent != m_event_times.end())
        {
            if(tevent->second < m_clock.getElapsedTime())
            {
                m_controllers.top()->timeEvent(tevent->first);
                tevent = m_event_times.erase(tevent);
            }
            else
            {
                tevent ++;
            }
        }
    }*/
}

