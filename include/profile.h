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

// Profile class for profile object
// Created by Charlie Carlson
// Scrum 1: Feb 18th 2014

// This class represents a user profile data
// It also includes the get and set functions which allow
// the game to change values

// Notes: at the moment the password is stored in plain text 

#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <cstdlib>
using std::free;

#include "badge.h"

#include <unordered_map>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

class Profile
{
public:
    
    enum gender {male, female};
    
    // Construct, which will create a class of a person who's name is NAME, age is 0 and has no password.
    Profile(std::wstring name = L"NAME", uint32_t age =0, gender gen = female, std::wstring password=L"password", std::wstring password_hint = L"password" );
   
    // Checks if password is correct
    bool checkPassword(const std::wstring&) const;
    // Sets password to something new (SUPER SECURE...not)
    void setPassword(const std::wstring&);
    
    // Returns name of profile
    std::wstring getName() const;
    // Sets name of profile
    void setName(std::wstring);
    
    // returns age of profile in years
    uint32_t getAge () const;
    // sets age of profile in years
    void setAge(uint32_t);
    
    gender getGender () const;
    void setGender(gender);
    
    // returns password hint as string
    std::wstring getPasswordHint() const;
    // sets the password hint
    void setPasswordHint(std::wstring);
    
    // returns current scence (string containing name of the scene for now)
    //std::wstring getCurrentScene() const;
    // Sets the current scene by name
    //void setCurrentScene(std::wstring);
    
    // adds a piece if a badge to gamestate/profile
    void addBadgePiece(Piece);

    // adds badge to gamestate/profile
    void addBadge(Badge);
    // remvoes badge from profile and saves game
    void removeBadge(Badge);
    
    // returns a list of badges for the current game state
    std::vector<Badge> getBadges();
    
    size_t getBadgeItemCount(); 
    
    // returns true if the badge is present in the current games tate
    bool lookupBadge(Badge) const;
    // same as above but takes badge name as string
    bool lookupBadge(std::wstring) const;
    bool lookupBadge(std::string) const;
    
    // returns the name of the file which contains the saved games for this profile
    // std::wstring getSavedGamesFile();
    // changes save game file TODO: Find a good naming convention
    //void setSavedGamesFile(std::wstring);
    
    // load in saved game
    void loadSavedGames();
    // save the list of games states to the savedgamesfile
    void saveSavedGames() const;
    
    // save the current game, making a new game state and pushing the other one
    // out to the file
    // void saveGame();
    
    // get the saved games for this profile from file
    //std::vector<std::shared_ptr<GameState>> getGameStates();
    // changes the current game state
    //void setGameState(std::shared_ptr<GameState>);
    
    template <class Archive>
	void save( Archive & ar ) const
	{
		ar( m_name, m_age, m_password_hint, m_password, m_badges);
	}
	
	template <class Archive>
	void load( Archive & ar )
	{
		ar( m_name, m_age, m_password_hint, m_password, m_badges);
        updateSavedFileName();
	}
private:
    void updateSavedFileName();
    
    std::wstring m_name;
    uint32_t m_age;
    
    gender m_gender;
    
    std::wstring m_password;
    std::wstring m_password_hint;
    
    std::wstring m_saved_games_file;
    
    std::unordered_map<std::wstring, Badge> m_badges;
    //GameState m_current_game;
    //std::vector<std::shared_ptr<GameState>> m_saved_games;
};


#endif /* end define for __PROFILE_H__ */ 
