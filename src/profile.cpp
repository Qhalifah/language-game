#include "profile.h"
#include "badge.h"

#include <string>
#include <unordered_map>
#include <fstream>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

Profile::Profile(std::wstring name , uint32_t age, gender gen, std::wstring password, std::wstring password_hint ): m_name(name), m_age(age), m_gender(gen), m_password(password), m_password_hint(password_hint), m_saved_games_file(name + std::to_wstring(age))
{
    //GameState new_game;
    //new_game.m_current_scene = L"scene"; //will be start scene when defined ...might be some clever way to read this from a file...like game settings
}

void Profile::updateSavedFileName()
{
    m_saved_games_file = (m_name + std::to_wstring(m_age));
}

bool Profile::checkPassword(const std::wstring& password) const
{
    return password == m_password;
}

void Profile::setPassword(const std::wstring& password)
{
    m_password = password;
}

std::wstring Profile::getPasswordHint() const
{
    return m_password_hint;
}

void Profile::setPasswordHint(std::wstring passwordHint)
{
    m_password_hint = passwordHint;
}

std::wstring Profile::getName() const
{
    return m_name;
}

void Profile::setName(std::wstring name)
{
    m_name = name;
    
    updateSavedFileName();
}

uint32_t Profile::getAge () const
{
    return m_age;
}

void Profile::setAge(uint32_t age)
{
    m_age = age;
    
    updateSavedFileName();
}

Profile::gender Profile::getGender () const
{
    return m_gender;
}

void Profile::setGender(Profile::gender gen)
{
    m_gender = gen; 
}

void Profile::addBadgePiece(Piece piece)
{
    if(m_badges.count(piece.m_badge_name))
    {
        m_badges[piece.m_badge_name].m_pieces.push_back(piece);
    }
}

void Profile::addBadge(Badge badge)
{
    m_badges.insert(std::make_pair(badge.m_name, badge));
}

void Profile::removeBadge(Badge badge)
{
    //saveGame();
     m_badges.erase(badge.m_name);
}

/*std::wstring Profile::getSavedGamesFile()
{
    return m_saved_games_file;
}

void Profile::setSavedGamesFile(std::wstring new_file)
{
    m_saved_games_file = new_file;
}*/

std::vector<Badge> Profile::getBadges()
{
    std::vector<Badge> temp;
    temp.reserve(m_badges.size());
    
    for(auto bg : m_badges)
    {
        temp.push_back(bg.second);
    }
    
    return temp;
}

size_t Profile::getBadgeItemCount()
{
    size_t t_count = 0;
    
    for(auto badge : getBadges())
    {
        if(badge.isComplete())
        {
            t_count++;
        }
        else
        {
            t_count += badge.m_pieces.size();
        }
    }
    
    return t_count;
}

bool Profile::lookupBadge(Badge badge) const
{
    return Profile::lookupBadge(badge.m_name);
}

bool Profile::lookupBadge(std::string badge) const
{
    return Profile::lookupBadge(std::wstring(badge.begin(), badge.end()));
}

bool Profile::lookupBadge(std::wstring badge_name) const
{
    //count for unordered map returns 1 or 0 so the cast is legit...
    return (bool)(m_badges.count(badge_name)) && m_badges.find(badge_name)->second.isComplete();
}

/*void Profile::loadSavedGames()
{
    std::wstring t_string= (L".//db//profilepage//" + m_saved_games_file + L".sg");
    std::string file_name;
    file_name.assign(t_string.begin(), t_string.end());
    std::ifstream is(file_name);
    
    if(!is.eof() && is)
    {
        cereal::BinaryInputArchive archive(is);
    
        archive( m_badges );
    }
    
    if(m_saved_games.size() > 0)
        m_current_game = *m_saved_games.back();
}

void Profile::saveSavedGames() const
{
    std::wstring t_string= (L".//db//profilepage//" + m_saved_games_file + L".sg");
    std::string file_name;
    file_name.assign(t_string.begin(), t_string.end());
    std::ofstream os(file_name);
    
    cereal::BinaryOutputArchive archive(os);
    
    archive( m_saved_games );
}*/

/*void Profile::saveGame()
{
    m_saved_games.push_back(std::make_shared<GameState>(m_current_game));
    saveSavedGames();
}

std::vector<std::shared_ptr<GameState>> Profile::getGameStates()
{
    return m_saved_games;
}

void Profile::setGameState(std::shared_ptr<GameState> game_state)
{
    m_current_game = *game_state;
}*/
