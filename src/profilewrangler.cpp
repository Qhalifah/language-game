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
			cout << "found profile to delete" << endl;
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
