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

// Scene.h
// Joseph DeWilde
// 2/24/2014

#ifndef SCENE_H
#define SCENE_H

#include "screen.h"
#include "badge.h"

#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <set>
using std::set;
#include <map>
using std::map;
#include <utility>
using std::pair;

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

enum GameType {MATCHING, PAIR};
typedef pair<GameType, string> MiniGame;

class Scene : public Screen
{
public:
    Scene(string filename, shared_ptr<Dictionary> dictionary = nullptr);
        Scene(float winW, float winH, string fileName);

    void setActivities(vector<MiniGame>& activities);
    void setRequirements(map<set<unsigned>, string>& requirements);
    void setBadge(Badge& badge);
    void setHelpMessage();
	MiniGame getMiniGameForActivityFileName(string);

    Badge badge();

    void save();
    void load();

    template <class Archive>
    void serialize( Archive & ar )
	{
		ar(m_screenItems,
			m_BGM,
			//m_dictionary,
			m_activities,
			m_requirements,
			m_badge
			);
		cout << "Scene serialize" << endl;
    }

    vector<MiniGame> m_activities;
	map<set<unsigned>, string> m_requirements;

private:
    string m_id;
    Badge m_badge;
};

#endif // SCENE_H
