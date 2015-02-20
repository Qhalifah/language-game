#include "profilewrangler.h"
#include "profile.h"

#include <iostream>

#include <vector>
#include <fstream>

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

ProfileWrangler::ProfileWrangler() : m_profiles(), m_curr_profile_index(-1)
{
    //std::cout << m_profiles.size() << std::endl;
}

std::vector<std::shared_ptr<Profile>> ProfileWrangler::loadProfiles(std::string fileName)
{
    std::ifstream is(".//db//profilepage//" + fileName + ".p",  std::ios::in | std::ios::binary );
    
    if (is.is_open() && !is.eof())
    {
        cereal::BinaryInputArchive archive(is);
        
        archive(m_profiles);
    }
    
    return m_profiles;
}

std::vector<std::shared_ptr<Profile>> ProfileWrangler::getProfiles()
{
    return m_profiles;
}

void ProfileWrangler::addProfile(std::shared_ptr<Profile> new_profile)
{
    m_profiles.push_back(new_profile);
    saveProfiles();
    loadProfiles();
}

void ProfileWrangler::addProfile(Profile new_profile)
{
    m_profiles.push_back(std::make_shared<Profile>(new_profile));
    saveProfiles();
    loadProfiles();
}

void ProfileWrangler::removeProfile(std::shared_ptr<Profile> old_profile)
{
    for(size_t pos = 0; pos < m_profiles.size(); ++pos)
    {
        if(m_profiles[pos] == old_profile)
        {
            m_profiles.erase(m_profiles.begin() + (int)pos);
        }
    }
    
    saveProfiles();
    loadProfiles();
}

void ProfileWrangler::removeProfile()
{
    m_profiles.erase(m_profiles.begin() + m_curr_profile_index);
    
    saveProfiles();
    loadProfiles();
}

void ProfileWrangler::saveProfiles(std::string fileName) const
{
    std::ofstream os(".//db//profilepage//" + fileName + ".p", std::ios::out | std::ios::binary | std::ios::trunc);

    if(os.is_open())
    {
        cereal::BinaryOutputArchive archive(os);
        
        archive( m_profiles);
    }
}

void ProfileWrangler::setCurrentProfile(int index)
{
    if((index > 0 && unsigned(index) > m_profiles.size()) || m_profiles.size() == 0)
        m_curr_profile_index = -1;
    else
        m_curr_profile_index = index;
}

std::shared_ptr<Profile> ProfileWrangler::getCurrentProfile()
{
    if(m_curr_profile_index > -1)
        return m_profiles[(size_t)m_curr_profile_index];
    else
        return NULL;
}
