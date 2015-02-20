#include "matchingcontroller.h"
#include "word.h"

minioncontroller_matching::minioncontroller_matching(std::shared_ptr<Profile> profile,
                                                     std::shared_ptr<Interface> interface,
                                                     std::shared_ptr<MasterController> masterController,
                                                     std::shared_ptr<Activity> activity)
                                                     : MinionController(profile, interface, masterController, activity), 
                                                     m_activity(activity), m_interfacePtr(interface)
{

    m_helpMessage  = L"In this game, I'll give you a bunch of words and then a\n";
    m_helpMessage += L"picture of one of them.All you have to do is click on the\n";
    m_helpMessage += L"word that matches the picture!Isn't that easy ? Just keep\n";
    m_helpMessage += L"doing it until you win!\n\n";
    m_helpMessage += L"Don't feel too bad if you mess up.You can replay these\n";
    m_helpMessage += L"games as often as you need.Have fun!";

    m_activity->setHelpMessage( m_helpMessage );

    m_grade = 0;
    m_engaged_sprite = 0;
    m_currentRound = 0;
    m_numOfScrItems = 4;
    m_indexOfInstrBox = 1;
    m_indexOfInstrText = 2;
    m_indexOfPicture = 3;
    m_numOfChoices = m_activity->getChoices();
    createInitialRound();
};


void minioncontroller_matching::mouseMovedEvent(sf::Event event, sf::Vector2i mouse_loc)
{
    int hit_sprite = m_interface->getScreenHit(mouse_loc.x, mouse_loc.y);
    if (hit_sprite > -1 && (size_t)hit_sprite > m_numOfScrItems-1)
    {
        ScreenItem m_screen_item;
        
        //Hit sprite is above 0 from above
        if (m_engaged_sprite != (size_t)hit_sprite && m_engaged_sprite > m_numOfScrItems-1)
        {
            m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
            
            m_screen_item.engaged = false;
            m_interface->update(m_engaged_sprite, m_screen_item);
        }
        
        m_engaged_sprite = (size_t)hit_sprite;
        m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
        
        m_screen_item.engaged = true;
        m_interface->update(m_engaged_sprite, m_screen_item);
        
    }
    else if (hit_sprite > -1 && (size_t)hit_sprite < m_numOfScrItems-1 && m_engaged_sprite > 0)
    {
        ScreenItem m_screen_item = m_screen->getScreenItems()[m_engaged_sprite];
            
        m_screen_item.engaged = false;
        m_interface->update(m_engaged_sprite, m_screen_item);
        m_engaged_sprite = (size_t)hit_sprite;
    }
}


void minioncontroller_matching::setGrade(size_t newGrade)
{
    m_grade = newGrade;
}


void minioncontroller_matching::mouseButtonReleasedEvent(sf::Event event, sf::Vector2i pos)
{
    if (gameIsOver())
    {
        m_master_controller->pop_controller();
    }
    else
    {
        int index = m_interfacePtr->getScreenHit(pos.x, pos.y);
        
        if(index > 0)
            m_interface->playScreenSound((size_t)index);

        // if index is between the unclickable screen items and the number of clickable text boxes
        if (index > 0 && (size_t)index > m_numOfScrItems-1 && index < (int)(m_numOfScrItems + m_numOfChoices))
        {
            checkAnswer((size_t)index);
        }
    }
}


size_t  minioncontroller_matching::getGrade()
{
    return m_grade;
}


void minioncontroller_matching::checkAnswer(size_t index)
{
    std::cout << "clicked box's index: " << index << "correct answer index: " << m_correctAnswerIndex + m_numOfScrItems << std::endl;
    //check index of clicked box against stored index of correct answer
    if (index == m_correctAnswerIndex + m_numOfScrItems)
    {
        ScreenItem t_screenitem;

        for (auto ii = unsigned(m_numOfScrItems + m_numOfChoices);
                ii < m_screen->getScreenItems().size(); ++ii)
        {
            if (!m_screen->getScreenItems()[ii].visible)
            {
                t_screenitem = m_screen->getScreenItems()[ii];
                t_screenitem.visible = true;
                m_screen->setScreenItem(ii, t_screenitem);
                m_interface->update(ii, t_screenitem);
                break;
            }
        }
        m_grade += 20;
        createNewRound();
    }
    else
    {
        //showCorrectImage(index); // If incorrect
        createNewRound();
    }
}

void minioncontroller_matching::populateRound()
{
    ++m_currentRound;

    if (gameIsOver())
    {
        endGame();
    }
    else
    {
        vector<ScreenItem> t_screenitems = m_screen->getScreenItems();
        vector<Word> t_wordlist;

        t_wordlist = getRandomWords(m_numOfChoices);
        for (size_t ii = 0; ii + m_numOfScrItems < m_numOfScrItems + m_numOfChoices; ++ii)
        {
            t_screenitems[ii + m_numOfScrItems].name = t_wordlist[ii].getDinName();
            
            if(m_profile->getGender() == Profile::female)
                 t_screenitems[ii + m_numOfScrItems].sound = t_wordlist[ii].getGirlAudioLocation();
            else
                 t_screenitems[ii + m_numOfScrItems].sound = t_wordlist[ii].getBoyAudioLocation();
            
            //t_screenitems[ii + m_numOfScrItems].sound = t_wordlist[ii].getGirlAudioLocation(); // TODO: NEED PROFILE CHECKING FOR BOY/GIRL
        }

        //randomly choose a selected word to be the correct answer
        srand(unsigned(time(NULL)));
        m_correctAnswerIndex = ((size_t)rand() % m_numOfChoices);
        t_screenitems[m_indexOfPicture].name = t_wordlist[m_correctAnswerIndex].getImageLocation();
        if(m_profile->getGender() == Profile::female)
            t_screenitems[m_indexOfPicture].sound = t_wordlist[m_correctAnswerIndex].getGirlAudioLocation();
        else
            t_screenitems[m_indexOfPicture].sound = t_wordlist[m_correctAnswerIndex].getBoyAudioLocation(); // TODO: NEED PROFILE CHECKING FOR BOY/GIRL
        m_screen->setScreenItems(t_screenitems);
    }
}

void minioncontroller_matching::createInitialRound()
{
    populateRound();
}


void minioncontroller_matching::createNewRound()
{
    populateRound();
    m_interface->update(m_numOfScrItems, m_numOfScrItems + m_numOfChoices,
                        m_screen->getScreenItems());
    m_interface->update(m_indexOfPicture,
                        m_screen->getScreenItems()[m_indexOfPicture]);
}


bool minioncontroller_matching::gameIsOver()
{
    return (m_currentRound > m_activity->getRounds());
}

void minioncontroller_matching::endGame()
{
    /*Badge t_badge;
    t_badge.m_name = L"FishBadge";
    t_badge.m_image  = L"images/fish.png";
    
    Piece t_piece;
    t_piece.m_id = 1;
    t_badge.m_image = L"images/rod.png";*/
    
    //m_profile->addBadge();
    m_profile->addBadgePiece(m_activity->getBadgePiece());
    
    vector<ScreenItem> t_screenitems = m_screen->getScreenItems();

    // change background screen to black
    t_screenitems[0].name = L"images/activity_endscreen.png";
    m_interface->update(0, t_screenitems[0]);

    // set box with player score to visible
    t_screenitems[m_indexOfInstrBox].visible = true;
    m_interface->update(m_indexOfInstrBox, t_screenitems[m_indexOfInstrBox]);
    // set text with player score to visible
    setGrade(m_grade);
    t_screenitems[m_indexOfInstrText].name = L"Your score is " + std::to_wstring(getGrade());
    t_screenitems[m_indexOfInstrText].size.x = 30;
    t_screenitems[m_indexOfInstrText].visible = true;
    m_interface->update(m_indexOfInstrText, t_screenitems[m_indexOfInstrText]);

    // set all other screen items to invisible
    for (auto ii = m_numOfScrItems-1; ii < t_screenitems.size(); ++ii)
    {
        t_screenitems[ii].visible = false;
        m_interface->update(ii, t_screenitems[ii]);
    }
    m_screen->setScreenItems(t_screenitems);
}
