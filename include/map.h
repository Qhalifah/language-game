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

// Map.h
// Joseph DeWilde
// 2/24/2014
#ifndef MAP_H
#define MAP_H

#include "screen.h"
#include "badge.h"
#include <string>
using std::string;
#include <map>
using std::unordered_map;
#include <set>
using std::set;
/*
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/set.hpp>
*/

#include "cereal\types\unordered_map.hpp"
#include "cereal\types\set.hpp"

class Map : public Screen
{
public:
    Map(string name, shared_ptr<Dictionary> dictionary = nullptr);
//  Map(float winW, float winH, string fileName);

    void setScenes(unordered_map<unsigned, string>& fileNames);
    void setScenePrereqs(unordered_map<unsigned, set<std::wstring>>& prereqs);
    void setHelpMessage();

    size_t getSceneListSize();
    const string& getScene(size_t index) const;

    const unordered_map<unsigned, set<std::wstring>>&
    scenePrereqs() const;

    void save();
    void load();

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(m_screenItems, m_BGM, 
			//m_dictionary, 
			m_scenes, m_scene_prereqs, m_scnPreRegsIds);
    }

    //unordered_map<unsigned, set<unsigned>> m_scnPreRegsIds;
private:
    string m_id;
    unordered_map<unsigned, string> m_scenes;
    unordered_map<unsigned, set<std::wstring>> m_scene_prereqs;
};

#endif // MAP_H
