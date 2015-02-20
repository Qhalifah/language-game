#include "badges.h"

#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
using cereal::PortableBinaryInputArchive;
using cereal::PortableBinaryOutputArchive;
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <unordered_map>
using std::unordered_map;
#include <QStringList>
#include <QDebug>
#include "choosefromlist.h"

Badges::Badges()
{
    load();
}

unsigned Badges::add(Badges::Badge &b)
{
    if(b.name.isEmpty())
        return 0;
    for(auto itr = m_badges.begin(); itr != m_badges.end(); ++itr)
    {
        if(itr.value().name == b.name)
            return 0; // already added
    }
    m_badges[++m_id] = b;
    save();
    return m_id;
}

bool Badges::remove(unsigned id)
{
    bool removed = m_badges.remove(id);
    save();
    return removed;
}

unsigned Badges::id(QString name)
{
    for(auto itr = m_badges.begin(); itr != m_badges.end(); ++itr)
    {
        if(itr.value().name == name)
            return itr.key();
    }
    return 0;
}

Badges::Badge Badges::badge(QString name)
{
    for(auto itr = m_badges.begin(); itr != m_badges.end(); ++itr)
    {
        if(itr.value().name == name)
            return itr.value();
    }
    return Badge();
}

Badges::Badge Badges::badge(unsigned id)
{
    if(!m_badges.contains(id))
        return Badge();
    return m_badges[id];
}

bool Badges::edit(unsigned id, Badges::Badge &b)
{
    if(!m_badges.contains(id))
        return false;
    m_badges[id] = b;
    save();
    return true;
}

QStringList Badges::list()
{
    if(m_badges.isEmpty())
        return QStringList();
    QStringList list;
    for(auto itr = m_badges.begin(); itr != m_badges.end(); ++itr)
    {
        list.append(itr.value().name);
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

QString Badges::badgeName(unsigned id)
{
    if(!m_badges.contains(id))
        return QString("");
    return m_badges[id].name;
}

unsigned Badges::getSelection()
{
    QStringList delList; // only need to pass to chooseFromList
    QString choice = chooseFromList(this->list(), delList, QString("Choose a Badge"), chooseFromList::ADD).getSelection();
    if(choice.isEmpty())
        return 0;
    return this->id(choice);
}

void Badges::save()
{
    m_badgesSave.clear();
    for(auto itr = m_badges.begin(); itr != m_badges.end(); ++itr)
    {
        Badges::BadgeSave bs(itr.value());
        m_badgesSave[itr.key()] = bs;
    }

    ofstream os(".//editor//badges.cereal");
    BinaryOutputArchive archive(os);
    archive(*this);
}

void Badges::load()
{
    m_badgesSave.clear();
    m_badges.clear();

    ifstream is(".//editor//badges.cereal");
    if(!is.is_open())
    {
        m_id = 0;
        return;
    }
    BinaryInputArchive archive(is);
    archive(*this);
    for(auto itr = m_badgesSave.begin(); itr != m_badgesSave.end(); ++itr)
    {
        m_badges[itr->first] = Badges::Badge(itr->second);
    }
}
