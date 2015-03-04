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

// Scene.cpp
// Joseph DeWilde
// 2/24/2014

#include "scene.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/tuple.hpp>
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
#include <fstream>
using std::ifstream;
using std::ofstream;


Scene::Scene(string name, shared_ptr<Dictionary> dictionary)
: Screen(dictionary), m_id(name)
{
}

Badge Scene::badge()
{
    return m_badge;
}

void Scene::save()
{
    std::ofstream os(".//db//scenes//" + m_id + ".scn");
    if( !os.fail() )
    {
        cereal::BinaryOutputArchive archive(os);
        archive(*this);
        /*archive(m_screenItems,
            m_BGM,
            m_dictionary,
            m_activities,
            m_requirements,
            m_badge
            );*/
    }
}

void Scene::load()
{
    std::ifstream is(".//db//scenes//" + m_id + ".scn");

    if(!is.eof() && is)
    {
        cereal::BinaryInputArchive archive(is);

        archive(*this);
        /*archive(m_screenItems,
            m_BGM,
            m_dictionary,
            m_activities,
            m_requirements,
            m_badge
            );*/
    }
}

void Scene::setActivities(vector<MiniGame>& activities)
{
    std::swap(m_activities, activities);
}

void Scene::setRequirements(map<set<unsigned>, unsigned>& requirements)
{
    std::swap(m_requirements, requirements);
}

void Scene::setBadge(Badge& badge)
{
    m_badge = badge;
}

void Scene::setHelpMessage()
{
    if (m_screenItems.size() > 2)
    {
        wstring helpMessage;
        helpMessage  = L"Why, hello again! Here we are at a fun location with all\n";
        helpMessage += L"sorts of activities to play. Again, just click the places\n";
        helpMessage += L"to head that way! You may have to click more than one,\n";
        helpMessage += L"but once you go there you'll get to all the fun!\n\n";

        helpMessage += L"Click the back arrow to head back to the main map, and\n";
        helpMessage += L"don't forget to click the '?' button if you need a quick\n";
        helpMessage += L"reminder of what to do. Have fun!";

        std::swap(m_screenItems[2].name, helpMessage);
    }
}


