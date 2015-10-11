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
//#include <algorithm>
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
#include "activity.h"

enum GameType {MATCHING, PAIR, NONE};
typedef pair<GameType, string> MiniGame; // <Activity's GameType, Activity's name>

class Scene : public Screen
{
public:
    Scene(string filename, shared_ptr<Dictionary> dictionary = nullptr);
//        Scene(float winW, float winH, string fileName);

    void setActivities(vector<MiniGame>& activities);
    void setRequirements(map<set<unsigned>,string>& requirements);
    void setBadge(Badge& badge);
    void setHelpMessage();
    void removeBadgeFromSelfAndActivityObjects();
    void updateBadgeInSelfAndActivityObjects(wstring, wstring, wstring);

    Badge badge();

    void save();
    void load();

    void renameBadge(wstring, wstring);

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
    }

    vector<MiniGame> m_activities;
   // map<set<unsigned>, unsigned> m_requirements;
    // Replacing the old m_requirements with one that uses the file names of the Activity objects,
    // because then deleting items from the m_activities vector will not cause the m_requirements vector to have bad data
    map<set<unsigned>, string> m_requirements;

    Badge m_badge;
private:
    string m_id;
};

#endif // SCENE_H
