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
    void removeScene(unsigned);
    void setScenePrereqs(unordered_map<unsigned, set<std::wstring>>& prereqs);
    void removeBadgeFromScenePrereqs(unsigned, wstring);
    void changePrereqBadgeName(wstring, wstring);
    void setHelpMessage();

    size_t getSceneListSize();
    const string& getScene(size_t index) const;

    const unordered_map<unsigned, set<std::wstring>>&scenePrereqs() const;

    void save();
    void load();

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(m_screenItems, m_BGM,
           //m_dictionary,
           m_scenes, m_scene_prereqs, m_scnPreRegsIds);
    }

    unordered_map<unsigned, set<unsigned>> m_scnPreRegsIds; // <screen_item_index, set of badge IDs>
    unordered_map<unsigned, set<std::wstring>> m_scene_prereqs; // <screen_item_index, set of badge names>
    unordered_map<unsigned, string> m_scenes; // <screen_item_index, Scene IDs>
private:
    string m_id;
};

#endif // MAP_H
