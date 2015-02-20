#include "profilewrangler.h"
#include "profile.h"
#include "badge.h"

#include <string>
#include <iostream>

SCENARIO( "Profile/Badges class", "[Profiles]" )
{
    GIVEN( "We have a Profile:" )
    {
        std::string name = "Test McTest";
        std::string password = "";
        std::string passwordhint = "";
        uint32_t age = 99;
        
        Profile m_profile0(name, age, password,  passwordhint);
        
        Badge m_badge0;
        m_badge0.m_name = "Test Badge0";
        Badge m_badge1;
        m_badge1.m_name = "Test Badge1";
        Badge m_badge2;
        m_badge2.m_name = "Test Badge2";
        Badge m_badge3;
        m_badge3.m_name = "Test Badge3";
        
        REQUIRE(m_profile0.getName() == name);
        REQUIRE(m_profile0.getAge() == age);
        REQUIRE(m_profile0.checkPassword(password));
        REQUIRE(m_profile0.getPasswordHint() == passwordhint);
        REQUIRE(m_profile0.getSavedGamesFile() == name+std::to_string(age).append(".xml"));
        REQUIRE(m_profile0.getGameStates().size() == 0);
        
        WHEN( "When we change the password:")
        {
            m_profile0.setPassword( "password");
            THEN( "The password changes and the old password should not work." )
            {
                REQUIRE( !m_profile0.checkPassword(password));
            }
            AND_THEN ("The new password should work.")
            {
                REQUIRE( m_profile0.checkPassword("password"));
            }
        }
 
        WHEN( "When we change the password hint:" )
        {
            m_profile0.setPasswordHint("New Password");
 
            THEN( "The password should have a new hint." )
            {
                REQUIRE( m_profile0.getPasswordHint() == "New Password");
            }
        }
        
        WHEN( "When we chnage the save game file:" )
        {
            m_profile0.setSavedGamesFile("mySavedGames.xml");
            
            THEN( "Then the saved game files should change." )
            {
                REQUIRE( m_profile0.getSavedGamesFile() == "mySavedGames.xml");
            }
        }
        
        WHEN( "When we change the name:" )
        {
            m_profile0.setName("Test McTest Jr.");
 
            THEN( "The name should change." )
            {
                REQUIRE(m_profile0.getName() == "Test McTest Jr.");
            }
        }
        
        WHEN ("When we change the age:")
        {
            m_profile0.setAge(100);
            
            THEN( "The age should change.")
            {
                REQUIRE(m_profile0.getAge() == 100);
            }
        }
        
        WHEN ("When we (save and) add badges:")
        {
            
            m_profile0.addBadge(m_badge0);
            
            m_profile0.addBadge(m_badge1);
            
            m_profile0.addBadge(m_badge2);

            m_profile0.addBadge(m_badge3);
            
            THEN("The amount of badges in the current game goes up.")
            {
                REQUIRE(m_profile0.getBadges().size() == 4);
                REQUIRE(m_profile0.getBadges().size() == 4);
            }
            AND_THEN("The badges we added should be present.")
            {
                REQUIRE(m_profile0.lookupBadge(m_badge0));
                REQUIRE(m_profile0.lookupBadge(m_badge0.m_name));
                REQUIRE(m_profile0.getBadges().size() == 4);
            }
        }
        AND_WHEN("When we remove badges from our game state:")
        {
            
            m_profile0.addBadge(m_badge0);
            
            m_profile0.addBadge(m_badge1);
            
            m_profile0.addBadge(m_badge2);
            
            m_profile0.addBadge(m_badge3);
            
            m_profile0.removeBadge(m_badge0);
            
            THEN("We should see the number of badges decrease.")
            {
                REQUIRE(m_profile0.getBadges().size() == 3);
            }
            AND_THEN("The badge should no longer be present")
            {
                REQUIRE(!m_profile0.lookupBadge(m_badge0));
                REQUIRE(!m_profile0.lookupBadge(m_badge0.m_name));
            }
        }
        
        WHEN("We create a new profile with the same game saved file, it should be able to load saved games.")
        {
            Profile m_profile1(m_profile0);
            
            m_profile0.saveSavedGames();
            m_profile1.loadSavedGames();

            
            THEN("Both profiles should have same number of saved games")
            {
                REQUIRE(m_profile0.getSavedGamesFile() == m_profile1.getSavedGamesFile());
                REQUIRE(m_profile1.getGameStates().size() == m_profile0.getGameStates().size());
            }
        }
        
        WHEN("We load in a profile we should be able to load previously saved games")
        {
            Profile m_profile1(m_profile0);
            m_profile1.saveGame();
            
            m_profile1.addBadge(m_badge0);
            
            THEN("We should see that we have made a new game state")
            {
                REQUIRE(m_profile1.getBadges().size() == 1);
            }
            
            m_profile1.setGameState(m_profile1.getGameStates().front());
            
            THEN("We should see that we have made a new game state")
            {
                REQUIRE(m_profile1.getBadges().size() == 0);
            }
            
        }
    }
 }

SCENARIO( "ProfileWrangler", "[Profiles]" )
{
    GIVEN( "We have a ProfileWrangler:" )
    {
        ProfileWrangler m_pw0;
        Profile m_profile0;
        
        WHEN("We add a profile")
        {
            m_pw0.addProfile(m_profile0);
            m_pw0.saveProfiles();
            
            THEN("We should have a profilewrangler with the profile")
            {
                REQUIRE(m_pw0.loadProfiles().size() == 1);
            }
        }
        
        WHEN("We add and remove a profile we should")
        {
            m_pw0.addProfile(m_profile0);
            m_pw0.addProfile(Profile("Another Tester"));
            m_pw0.saveProfiles();
            
            THEN("see the number grow")
            {
                REQUIRE(m_pw0.loadProfiles().size() == 2);
            }
            
            m_pw0.removeProfile(m_pw0.loadProfiles().front());
            
            THEN("see the number grow")
            {
                REQUIRE(m_pw0.loadProfiles().size() == 1);
                REQUIRE(m_pw0.loadProfiles().front()->getName() == "Another Tester");
            }
        }
    }
}











