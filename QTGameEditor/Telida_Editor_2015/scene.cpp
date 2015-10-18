// Scene.cpp
// Joseph DeWilde
// 2/24/2014

#include "scene.h"

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
    if(m_screenItems[0].name == L"")
    {
        cout << "No background image, putting None in name so no crash occurrs." << endl;
        m_screenItems[0].name = L"None";
    }


    // Get the user's Documents folder path
    wchar_t cStr[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, DWORD("FOLDERID_Documents"), cStr);
    std::string path = QString::fromStdWString(cStr).toStdString();


    std::ofstream os(m_DocumentsPath + "//Dinaki Adventures//db//scenes//" + m_id + ".scn", std::ios::binary | std::ios_base::binary | ofstream::out | std::ios::trunc);
    QString qS = QString::fromStdWString(m_badge.m_name);
    cout << "Scene badge name: "<< qS.toStdString() << endl;
    if( !os.fail() )
    {
        cereal::BinaryOutputArchive archive(os);

        archive(*this);
    }
}

void Scene::load()
{
    cout << "in scene load" << endl;
    std::ifstream is(m_DocumentsPath+"//Dinaki Adventures//db//scenes//" + m_id + ".scn", std::ios::binary | std::ios_base::binary | ifstream::in);

    if(!is.eof() && is)
    {
        cereal::BinaryInputArchive archive(is);

        archive(*this);
    }
    cout << "Scene's m_id is " << m_id << endl;
    for(int ii=0; ii<m_activities.size(); ++ii)
    {
        cout << "m_activities[ii].m_name: " << m_activities[ii].second << endl;
    }
}

void Scene::setActivities(vector<MiniGame>& activities)
{
    std::swap(m_activities, activities);
}

void Scene::setRequirements(map<set<unsigned>, string>& requirements)
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

void Scene::removeBadgeFromSelfAndActivityObjects()
{
    m_badge = Badge();

    // Delete Badge from Activity objects
    for(auto jj : m_activities)
    {
        Activity act(jj.second, NULL);
        act.load();
        QString qStr = QString::fromStdWString(act.m_badge_piece.m_badge_name);
        cout << "Deleting Badge Piece, " << qStr.toStdString() << " for " << jj.second << endl;
        act.m_badge_piece.m_badge_name = L"none";
        act.m_badge_piece.m_id = 1;
        act.m_badge_piece.s_id = "none";
        act.save();
    }
}

void Scene::updateBadgeInSelfAndActivityObjects(wstring originalBadgeName, wstring newBadgeName, wstring imageLocation)
{
    m_badge.m_name = newBadgeName;
    m_badge.m_image = imageLocation;

    // Change Badge in Activity objects
    for(auto jj : m_activities)
    {
        Activity act(jj.second, NULL);
        act.load();
        QString qStr = QString::fromStdWString(act.m_badge_piece.m_badge_name);
        cout << "Deleting Badge Piece, " << qStr.toStdString() << " for " << jj.second << endl;
        act.m_badge_piece.m_badge_name = newBadgeName;
        act.save();
    }
}

void Scene::renameBadge(wstring old_badge_name, wstring new_badge_name)
{
    if(m_badge.m_name == old_badge_name)
    {
        m_badge.m_name = new_badge_name;

        // Save the badge name to the pieces of all Activity objects used in the scene
        for(size_t i = 0; i < m_activities.size(); ++i)
        {
            if(m_activities[i].first != GameType::NONE)
            {
                QString qActFileName = QString::fromStdString(m_activities[i].second);
                string actFileName = qActFileName.toStdString();
                cout << "Saving data for the Activity " << actFileName << endl;
                Activity *saveAct = new Activity(actFileName, NULL);
                saveAct->load();
                saveAct->m_badge_piece.m_badge_name = new_badge_name;
                saveAct->save();
            }
        }
    }
}
