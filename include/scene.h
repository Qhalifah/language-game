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

enum GameType {MATCHING, PAIR};
typedef pair<GameType, string> MiniGame;

class Scene : public Screen
{
public:
    Scene(string filename, shared_ptr<Dictionary> dictionary = nullptr);
//        Scene(float winW, float winH, string fileName);

    void setActivities(vector<MiniGame>& activities);
    void setRequirements(map<set<unsigned>,unsigned>& requirements);
    void setBadge(Badge& badge);
    void setHelpMessage();

    Badge badge();

    void save();
    void load();

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(m_screenItems,
            m_BGM,
            m_dictionary,
            m_activities,
            m_requirements,
            m_badge
          );
    }

    vector<MiniGame> m_activities;
    map<set<unsigned>, unsigned> m_requirements;

private:
    string m_id;
    Badge m_badge;

};

#endif // SCENE_H
