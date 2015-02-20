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
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/set.hpp>

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
        ar(m_screenItems, m_BGM, m_dictionary, m_scenes, m_scene_prereqs, m_scnPreRegsIds);
    }

    unordered_map<unsigned, set<unsigned>> m_scnPreRegsIds;
    unordered_map<unsigned, set<std::wstring>> m_scene_prereqs;
    unordered_map<unsigned, string> m_scenes;
private:
    string m_id;


};

#endif // MAP_H
