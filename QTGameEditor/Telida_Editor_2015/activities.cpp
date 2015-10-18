#include "activities.h"
#include <string>
using std::string;
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <QMap>
#include <QString>
#include "choosefromlist.h"

Activities::Activities(QString filename):m_filename(filename)
{
    // Get the user's Documents folder path
    SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, DWORD("FOLDERID_Documents"), cStr);
    m_DocumentsPath = QString::fromStdWString(cStr).toStdString();

    m_id = 0;
    ActSave actSave;
    m_acts.clear();
    load();
}

unsigned Activities::add(Activity act)
{
    // If the act's name is empty, return
    if(act.name.isEmpty())
        return 0;
    // If the act alreasy exists in the Activities object, return
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        if(itr.value().name == act.name)
            return 0;
    }
    // act is new and has text, put it in the Activities object and increment the Activities object's m_id
    m_acts[++m_id] = act;
    save();
    return m_id;
}

QString Activities::getSelection()
{
    QStringList delList;
    QStringList list;
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        list.append(itr.value().name);
    }
    QString selection = chooseFromList(list, delList, QString("Choose an Activity")).getSelection();
    for(auto itr = delList.begin(); itr != delList.end(); ++itr)
    {
        this->remove(*itr);
    }
    save();
    return selection;
}

bool Activities::edit(unsigned id, Activity act)
{
    if(!m_acts.contains(id) || act.name.isEmpty())
    {
        return false;
    }
    if(m_acts[id].name == act.name)
    {
        m_acts[id] = act;
        return true;
    }
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        if(itr.value().name == act.name)
        {
            return false;
        }
    }
    m_acts[id] = act;
    save();
    return true;
}

QStringList Activities::list()
{
    QStringList list;
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        list.append(itr.value().name);
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

unsigned Activities::id(QString name)
{
    if(name.isEmpty())
        return 0;
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        if(itr.value().name == name)
        {
            return itr.key();
        }
    }
    return 0; // Didn't find id
}

Activities::Activity Activities::act(unsigned id)
{
    if(!m_acts.contains(id))
    {
        return Activity();
    }
    return m_acts[id];
}

QStringList Activities::listWithName(QString name)
{
    QStringList normList = this->list();
    QStringList list;
    for(auto itr = normList.begin(); itr != normList.end(); ++itr)
    {
        QString actName = name;
        actName += *itr;
        list.append(actName);
    }
    return list;
}

bool Activities::remove(QString name)
{
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        cout << "removing " << name.toStdString() << endl;
        if(itr.value().name == name)
        {
            cout << "removing " << name.toStdString() << endl;
            m_acts.remove(itr.key());
            save();
            return true;
        }
    }
    return false;
}

void Activities::save()
{
    cout << "activities save for " + m_filename.toStdString() << endl;
    m_actSave.clear();
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        m_actSave[itr.key()] = Activities::ActSave(itr.value());
    }

    ofstream os(m_DocumentsPath+"//Dinaki Adventures//editor//" + m_filename.toStdString() + ".cereal", std::ios::binary); //);
    cereal::BinaryOutputArchive archive(os);
    archive(*this);
}

void Activities::load()
{
    cout << "activites load strarted." << endl;

    m_acts.clear();
    m_actSave.clear();

    cout << "above ifstream: " << m_DocumentsPath+"//Dinaki Adventures//editor//" + m_filename.toStdString()+ ".cereal" << endl;
    ifstream is(m_DocumentsPath+"//Dinaki Adventures//editor//"+m_filename.toStdString()+".cereal", std::ios::binary);
    cout << "above if" << endl;
    if(!is.is_open())
    {
        cout << "Activities couldn't load" << endl;
        m_id = 0;
        return;
    }
    cout << "Activities loaded" << endl;
    cereal::BinaryInputArchive archive(is);
    cout << "Archive created" << endl;
    archive(*this);

    cout << "above for loop" << endl;
    for(auto itr = m_actSave.begin(); itr != m_actSave.end(); ++itr)
    {
        m_acts[itr->first] = Activities::Activity(itr->second);
    }

    cout << "activites load complete. Size of act array is " << m_acts.size() << endl;
}
