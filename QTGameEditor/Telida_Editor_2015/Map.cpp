// Map.cpp
// Joseph DeWilde
// 2/24/2014
#include "map.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
using cereal::PortableBinaryInputArchive;
using cereal::PortableBinaryOutputArchive;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <string>
#include <iostream>
using std::ios_base;
#include <utility>
using std::make_pair;

Map::Map(string name, shared_ptr<Dictionary> dictionary)
: Screen(dictionary), m_id(name)
{
}

//Map::Map(float winW, float winH, string fileName)
//{
//    loadFromFile(winW, winH, fileName);
//}

void Map::setScenes(unordered_map<unsigned, string>& fileNames)
{
    swap(m_scenes, fileNames);
}

void Map::removeScene(unsigned sceneID)
{
    //Remove the Scene from m_scenes
    cout << "m_scenes.size() is " << m_scenes.size() << endl;
    cout << "m_scnPreRegsIds.size() is " << m_scnPreRegsIds.size() << endl;
    cout << "m_scene_prereqs.size() is " << m_scene_prereqs.size() << endl;
    for(auto itr : m_scenes)
    {
        if(itr.second == std::to_string(sceneID))
        {
            m_scnPreRegsIds.erase(itr.first);
            m_scene_prereqs.erase(itr.first);
            m_scenes.erase(itr.first);
            break;
        }
    }
    cout << "m_scenes.size() is " << m_scenes.size() << endl;
    cout << "m_scnPreRegsIds.size() is " << m_scnPreRegsIds.size() << endl;
    cout << "m_scene_prereqs.size() is " << m_scene_prereqs.size() << endl;
}

void Map::setScenePrereqs(unordered_map<unsigned, set<std::wstring>>& prereqs)
{
    swap(m_scene_prereqs, prereqs);
}

void Map::removeBadgeFromScenePrereqs(unsigned badgeId, wstring badgeName)
{
    for(auto ii= m_scnPreRegsIds.begin(); ii!= m_scnPreRegsIds.end(); ++ii)
    {
        for(auto jj : (*ii).second)
        {
            if(jj == badgeId)
            {
                unsigned iiSave = (*ii).first;
                cout << "Deleting m_scnPreRegsIds " << jj << endl;
                (*ii).second.erase(jj);

                for(auto kk : m_scene_prereqs[iiSave])
                {
                    if(kk == badgeName)
                    {
                        QString qStr = QString::fromStdWString(badgeName);
                        cout << "Deleting badge from m_scene_prereqs " << qStr.toStdString() << endl;
                        m_scene_prereqs[iiSave].erase(kk);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void Map::changePrereqBadgeName(std::wstring originalBadgeName, std::wstring newBadgeName)
{
    for(auto ii= m_scene_prereqs.begin(); ii!= m_scene_prereqs.end(); ++ii)
    {
        for(auto kk : (*ii).second)
        {
            if(kk == originalBadgeName)
            {
                kk = newBadgeName;
                break;
            }
        }
    }
}

void Map::setHelpMessage()
{
    if (m_screenItems.size() > 2)
    {
        wstring helpMessage;
        helpMessage  = L"Hey there! This is the main map where you can pick where\n";
        helpMessage += L"you would like to go! Just click on any of those places to\n";
        helpMessage += L"see where they take you!\n\n";

        helpMessage += L"Oh, and if you EVER forget anything I tell you, click on\n";
        helpMessage += L"that '?' button in the upper left to call me right back\n";
        helpMessage += L"so I can remind you about how to play. Have fun!";
        std::swap(m_screenItems[2].name, helpMessage);
    }
}

size_t Map::getSceneListSize()
{
    return m_scenes.size();
}

const string& Map::getScene(size_t index) const
{
    static const string nullstr = "";
    if(m_scenes.count(index))
        return m_scenes.at(index);
    else
        return nullstr;
}

const unordered_map<unsigned, set<std::wstring>>&
Map::scenePrereqs() const
{
    return m_scene_prereqs;
}

void Map::save()
{
    if(m_screenItems[0].name == L"")
    {
        cout << "No background image, putting None in name so no crash occurrs." << endl;
        m_screenItems[0].name = L"None";
    }

    std::ofstream os(".//db//maps//" + m_id + ".map", std::ios::binary | std::ios_base::binary | ofstream::out | std::ios::trunc);

    cereal::PortableBinaryOutputArchive archive(os);

    archive(*this);
    //os.close();
}

void Map::load()
{
    cout << "in map load" << endl;
    std::ifstream is(".//db//maps//" + m_id + ".map", std::ios::binary | std::ios_base::binary | ifstream::in);

    if(!is.eof() && is)
    {
        cereal::PortableBinaryInputArchive archive(is);

        archive(*this);
    }

    cout << "m_scnPreRegsIds.size(): " << m_scnPreRegsIds.size() << endl;
    /*for(auto ii= m_scnPreRegsIds.begin(); ii!= m_scnPreRegsIds.end(); ++ii)
    {
        for(auto jj : (*ii).second)
        {
            //QString qStr = QString::fromStdWString(*jj);
            cout << ", m_scnPreRegsIds " << jj << endl;
        }
    }*/
}

