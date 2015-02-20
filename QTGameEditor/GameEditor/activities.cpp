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
    load();
}

unsigned Activities::add(Activity act)
{
    if(act.name.isEmpty())
        return 0;
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        if(itr.value().name == act.name)
            return 0;
    }
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
        if(itr.value().name == name)
        {
            m_acts.remove(itr.key());
            save();
            return true;
        }
    }
    return false;
}

void Activities::save()
{
    m_actSave.clear();
    for(auto itr = m_acts.begin(); itr != m_acts.end(); ++itr)
    {
        m_actSave[itr.key()] = Activities::ActSave(itr.value());
    }

    ofstream os(".//editor//" + m_filename.toStdString() + ".cereal");
    cereal::BinaryOutputArchive archive(os);
    archive(*this);
}

void Activities::load()
{
    m_acts.clear();
    m_actSave.clear();
    ifstream is(".//editor//"+m_filename.toStdString()+".cereal");
    if(!is.is_open())
    {
        m_id = 0;
        return;
    }
    cereal::BinaryInputArchive archive(is);
    archive(*this);

    for(auto itr = m_actSave.begin(); itr != m_actSave.end(); ++itr)
    {
        m_acts[itr->first] = Activities::Activity(itr->second);
    }
}
