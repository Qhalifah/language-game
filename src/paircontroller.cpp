#include "paircontroller.h"
#include "screen.h"
#include "interface.h"
#include "scene.h"
#include "dictionary.h"
#include "word.h"

#include <vector>
#include <iostream>
#include <string>
using std::to_wstring;

void PairController::randomizeWords(unsigned int start, unsigned int end)
{
    std::vector<ScreenItem> t_screen_items = m_screen->getScreenItems();
    Word t_word;
    
    Vec2 bgSize = t_screen_items[0].size;
    float width = std::min(bgSize.x / (m_count*.5 + 1), bgSize.y * .2125);
    float spacer = (bgSize.x - width*m_count*.5) / (m_count*.5 + 1);
    float y1 = bgSize.y * .025;
    float y2 = bgSize.y * .6;
    float x  = spacer;
    float deltaX = (width + spacer);
    vector<unsigned> images;
    images.reserve(m_count);
    vector<unsigned> words;
    words.reserve(m_count);

    // Load the vector with all the possible indices for images/words
    for(unsigned int iter = start; iter < end; iter+=2)
    {
        images.push_back(iter);
        words.push_back(iter+1);
    }

    set<wstring> wordset;
    while( !images.empty() && wordset.size() < m_screen->getDictionary()->getSize() )
    {
        // Get a unique random word from dictionary
        while( true )
        {
            t_word = m_screen->getDictionary()->getRandomWord();
            auto insert = wordset.insert(t_word.getDinName());
            if( insert.second == true )
                break;
        }

        // Remove two indices at random. Store image at one, word at the other.
        size_t index = (size_t)rand() % images.size();
        unsigned image = images[index];
        images.erase(images.begin()+(int)index);
        index = (size_t)rand() % words.size();
        unsigned word  = words[index];
        words.erase(words.begin()+(int)index);

        t_screen_items[image].name = t_word.getImageLocation();
        
        if(m_profile->getGender() == Profile::female)
            t_screen_items[image].sound = t_word.getGirlAudioLocation();
        else
            t_screen_items[image].sound = t_word.getBoyAudioLocation();
        t_screen_items[image].volume = 100;
        t_screen_items[image].size = Vec2(width, width);
        t_screen_items[image].position = Vec2(x+deltaX*((image-start)/4),
                                              y1+(image/2%2)*(width+y1));
        t_screen_items[image].behavior = 24;
        
        t_screen_items[word].type = TEXT;
        t_screen_items[word].name = t_word.getDinName();
        
        if(m_profile->getGender() == Profile::female)
            t_screen_items[word].sound = t_word.getGirlAudioLocation();
        else
            t_screen_items[word].sound = t_word.getBoyAudioLocation();
        
        t_screen_items[word].position = Vec2(x+deltaX*((word-start-1)/4),
                                             y2+(word/2%2)*width);
        t_screen_items[word].size = {20, 0};
        t_screen_items[word].color = Color::Black;
        t_screen_items[word].behavior = 24;

        // Save these two items as a pair
        m_pairs.emplace(set<unsigned>({image, word}));
    }
    
    m_screen->setScreenItems(t_screen_items);
}

PairController::PairController(std::shared_ptr<Profile> profile,
                std::shared_ptr<Interface> interface,
                std::shared_ptr<MasterController> masterController,
                std::shared_ptr<Activity> activity)
                : MinionController(profile, interface, masterController, activity),
                  m_engaged_sprite(0), m_answered(0), m_tries(0),
                  m_activity(activity)
{

    m_helpMessage =  L"In this game, I'll give you a bunch of words and pictures\n";
    m_helpMessage += L"of each of them. All you have to do is click on each word\n";
    m_helpMessage += L"and its matching picture! Isn't that easy? Just keep doing\n";
    m_helpMessage += L"it until you win!\n\n";
    m_helpMessage += L"Don't feel too bad if you mess up. You can replay these\n";
    m_helpMessage += L"games as often as you want. Have fun!";

    m_activity->setHelpMessage(m_helpMessage);

    std::vector<ScreenItem> t_screen_items = m_screen->getScreenItems();
    
    ScreenItem t_screen_item;
    t_screen_item.type = IMAGE;
    t_screen_item.name = L"images/fish.png";
    t_screen_item.size = m_screen->getScreenItems()[0].size * .05;
    //t_screen_item.sound = L"audio/other/splash.wav";
    t_screen_item.visible = false;
    
    m_count = m_activity->getChoices();
    size_t initial_size = t_screen_items.size();

    // Make room for images + words
    t_screen_items.resize(initial_size+(2*m_count));

    srand(unsigned(time(NULL)));
    // Push the rewards on the back
    for( unsigned ii = 0; ii < m_count; ++ii )
    {
        t_screen_item.position.x = m_screen->getScreenItems()[0].size.x * .4
                                 * (1.0 + (rand() % 100) / 200.0);
        t_screen_item.position.y = m_screen->getScreenItems()[0].size.y * .6
                                 * (1.0 + (rand() % 100) / 200.0);
        t_screen_items.push_back(t_screen_item);
    }
    m_screen->setScreenItems(t_screen_items);
    randomizeWords(initial_size, initial_size + 2*m_count);

    /*t_screen_items = m_screen->getScreenItems();
    for(auto ii: t_screen_items)
    {
        std::wcout << ii.name << std::endl;
    }*/
};

void PairController::mouseButtonPressedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    
     if(hit_sprite > -1)
    {
        ScreenItem screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];
        m_interface->playScreenSound((size_t)hit_sprite);
    }
}

void PairController::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

    if (m_engaged_sprite != hit_sprite && hit_sprite > -1)
    {// Then we are over a sprite, and it is not the currently engaged one.
        ScreenItem screen_item;

        if (m_engaged_sprite > -1)
        {// Then there is a sprite which is currently engaged.
            // Disengage the currently engaged sprite
            screen_item = m_screen->getScreenItems()[(size_t)m_engaged_sprite];
            disengageItem((size_t)m_engaged_sprite);
            m_engaged_sprite = -1;
        }

        screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];

        if (screen_item.behavior & HOVER_ENGAGE)
        {// Then the screen item is engageable on hover
            screen_item.engaged = true;
            screen_item.hover = true;

            m_screen->setScreenItem((size_t)hit_sprite, screen_item);
            m_interface->update((size_t)hit_sprite, screen_item);
            m_engaged_sprite = hit_sprite;
        }

        if (screen_item.behavior & HOVER_SELECT)
        {
            //TODO: decided if this will be implemented
            //m_interface->update(hit_sprite, screen_item);
            //m_engaged_sprite = hit_sprite;
        }
    }
}


void PairController::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    if (gameIsOver())
    {
        m_master_controller->pop_controller();
    }
    else
    {
        int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);

        if (hit_sprite > -1)
        {
            if (unsigned(hit_sprite) < (m_screen->getScreenItems().size() - m_count))
            {
                ScreenItem screen_item = m_screen->getScreenItems()[(size_t)hit_sprite];
                if( screen_item.behavior & MOUSE_UP_SELECT )
                {
                    screen_item.selected = !screen_item.selected;

                    if (screen_item.selected)
                    {// Add it to the set
                        m_selected.insert((size_t)hit_sprite);
                        screen_item.color = sf::Color::Green;
                        m_screen->setScreenItem((size_t)hit_sprite, screen_item);
                        m_interface->update((size_t)hit_sprite, screen_item);

                        if (m_selected.size() == 2)
                        {
                            auto iter = m_selected.begin();
                            unsigned int sprite1 = *iter;
                            unsigned int sprite2 = *(++iter);
                            auto screen_item1 = m_screen->getScreenItems()[sprite1];
                            auto screen_item2 = m_screen->getScreenItems()[sprite2];

                            if (((sprite1 + sprite2) % 2) == 0)
                            { //Both even or both odd, so both images or both words.
                                if ((size_t)hit_sprite == sprite1)
                                {// Just added, erase the other.
                                    screen_item2.selected = false;
                                    screen_item2.color = sf::Color::Green;
                                    m_screen->setScreenItem(sprite2, screen_item2);
                                    m_interface->update(sprite2, screen_item2);
                                    m_selected.erase(iter);
                                }
                                else
                                {// Other was added, erase this one.
                                    screen_item1.selected = false;
                                    screen_item1.color = sf::Color::Green;
                                    m_screen->setScreenItem(sprite1, screen_item1);
                                    m_interface->update(sprite1, screen_item1);
                                    m_selected.erase(--iter);
                                }
                            }
                            else
                            {
                                // Look for a match, and erase selection either way
                                auto match = m_pairs.find(m_selected);
                                if (match != m_pairs.end())
                                {// A match was found, remove it
                                    screen_item1.visible = false;
                                    screen_item2.visible = false;
                                    m_pairs.erase(match);
                                    auto index = m_screen->getScreenItems().size() - m_answered - 1;
                                    auto reward = m_screen->getScreenItems()[index];
                                    reward.visible = true;
                                    m_screen->setScreenItem(index, reward);
                                    m_interface->update(index, reward);
                                    ++m_answered;
                                }

                                // Erase selection
                                screen_item1.selected = false;
                                screen_item1.color = sf::Color::White;
                                m_screen->setScreenItem(sprite1, screen_item1);
                                m_interface->update(sprite1, screen_item1);

                                screen_item2.selected = false;
                                screen_item2.color = sf::Color::White;
                                m_screen->setScreenItem(sprite2, screen_item2);
                                m_interface->update(sprite2, screen_item2);

                                m_selected.clear();
                                ++m_tries;
                            }
                        }

                    }
                    else
                    {// Remove it from the selection
                        auto sprite = m_selected.find((size_t)hit_sprite);
                        m_selected.erase(sprite);
                        screen_item.color = sf::Color::White;
                        m_screen->setScreenItem((size_t)hit_sprite, screen_item);
                        m_interface->update((size_t)hit_sprite, screen_item);
                    }
                }
            }
            else
            {

            }
        }
        if (gameIsOver())
            endGame();
    }
}

bool PairController::gameIsOver()
{
    return m_pairs.size() == 0;
}

void PairController::endGame()
{
    /*Badge t_badge;
    t_badge.m_name = L"FishBadge";
    t_badge.m_image  = L"images/fish.png";
    
    Piece t_piece;
    t_piece.m_id = 0;
    t_badge.m_image = L"images/rod.png";*/
    
    //m_profile->addBadge(t_badge);
    //m_profile->addBadgePiece(t_badge,t_piece);
    
    //m_profile->addBadgePiece(m_activity->m_badge, m_activity->m_badge_piece);
    m_profile->addBadgePiece(m_activity->getBadgePiece());
    
    vector<ScreenItem> t_screenitems = m_screen->getScreenItems();

    // change background screen to black
    t_screenitems[0].name = L"images/activity_endscreen.png";
    m_interface->update(0, t_screenitems[0]);

    // set box with player score to visible
    t_screenitems[1].visible = true;
    m_interface->update(1, t_screenitems[1]);

    // set text with player score to visible
    t_screenitems[2].name = L"Your score is " + to_wstring(int((m_answered*100.0)/m_tries));
    t_screenitems[2].size.x = 30;
    t_screenitems[2].visible = true;
    m_interface->update(2, t_screenitems[2]);

    // set all other screen items to invisible
    for (auto ii = 3u; ii < t_screenitems.size(); ++ii)
    {
        t_screenitems[ii].visible = false;
        m_interface->update(ii, t_screenitems[ii]);
    }
    m_screen->setScreenItems(t_screenitems);
}
