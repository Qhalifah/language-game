#include "dbmap.h"
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
#include <qdebug.h>

#include <QMap>
#include <QString>
#include "choosefromlist.h"

DBMap::DBMap(string filename)
{
    load(filename);
}

DBMap::~DBMap()
{}

unsigned DBMap::add(QString name)
{
    if(m_dbMap.values().contains(name))
    {
        return 0;
    }
    m_dbMap[++m_id] = name;
    save();
    return m_id;
}

QString DBMap::getSelection()
{
    QStringList delList;
    QString selection = chooseFromList(m_dbMap.values(), delList, QString("Choose a Scene")).getSelection();
    for(auto itr = delList.begin(); itr != delList.end(); ++itr)
    {
        this->remove(*itr);
    }
    save();
    return selection;
}

bool DBMap::rename(unsigned id, QString name)
{
    if(!m_dbMap.contains(id) || name.isEmpty())
    {
        return false;
    }
    if(m_dbMap[id] == name)
    {
        return true;
    }
    for(auto itr = m_dbMap.begin(); itr != m_dbMap.end(); ++itr)
    {
        if(itr.value() == name)
        {
            return false;
        }
    }
    m_dbMap[id] = name;
    save();
    return true;
}

void DBMap::rename(QString oldName, QString newName)
{
    for(auto itr = m_dbMap.begin(); itr != m_dbMap.end(); ++itr)
    {
        if(itr.value() == oldName)
        {
            m_dbMap[itr.key()] = newName;
            break;
        }
    }
    save();
}

QStringList DBMap::list()
{
    QStringList list = m_dbMap.values();
    list.sort(Qt::CaseInsensitive);
    return list;
}

unsigned DBMap::id(QString name)
{
    if(name.isEmpty())
        return 0;
    for(auto itr = m_dbMap.begin(); itr != m_dbMap.end(); ++itr)
    {
        if(itr.value() == name)
        {
            return itr.key();
        }
    }
    return 0; // Didn't find id
}

QString DBMap::name(unsigned id)
{
    if(!m_dbMap.contains(id))
    {
        return QString("");
    }
    return m_dbMap[id];
}

void DBMap::remove(QString name)
{
    for(auto itr = m_dbMap.begin(); itr != m_dbMap.end(); ++itr)
    {
        if(itr.value() == name)
        {
            m_dbMap.remove(itr.key());
            break;
        }
    }
    save();
}

void DBMap::save()
{
    m_dbMapSave.clear();
    for(auto itr = m_dbMap.begin(); itr != m_dbMap.end(); ++itr)
    {
        m_dbMapSave[itr.key()] = itr.value().toStdString();
    }

    ofstream os(".//editor//scenes.cereal");
    cereal::BinaryOutputArchive archive(os);
    archive(*this);
}

void DBMap::load(string filename)
{
    m_dbMap.clear();
    m_dbMapSave.clear();
    ifstream is(".//editor//" + filename + ".cereal");
    if(!is.is_open())
    {
        m_id = 0;
        return;
    }
    cereal::BinaryInputArchive archive(is);
    archive(*this);

    for(auto itr = m_dbMapSave.begin(); itr != m_dbMapSave.end(); ++itr)
    {
        m_dbMap[itr->first] = QString::fromStdString(itr->second);
    }
}
