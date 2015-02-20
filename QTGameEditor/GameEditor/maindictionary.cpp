#include "maindictionary.h"

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

MainDictionary::MainDictionary()
{
    load();
}

unsigned MainDictionary::add(MainDictionary::Word &w)
{
    if(w.name.isEmpty())
        return 0;
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        if(itr.value().name == w.name)
            return 0; // already added
    }
    m_dict[++m_id] = w;
    save();
    return m_id;
}

unsigned MainDictionary::add(QString name, QString img, QString boySnd, QString girlSnd)
{
    if(name.isEmpty())
        return 0;
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        if(itr.value().name == name)
            return 0; // already added
    }
    m_dict[++m_id] = Word(name, img, boySnd, girlSnd);
    save();
    return m_id;
}

bool MainDictionary::remove(unsigned id)
{
    bool removed = m_dict.remove(id);
    save();
    return removed;
}

unsigned MainDictionary::id(QString name)
{
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        if(itr.value().name == name)
            return itr.key();
    }
    return 0;
}

MainDictionary::Word MainDictionary::word(QString name)
{
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        if(itr.value().name == name)
            return itr.value();
    }
    return Word();
}

MainDictionary::Word MainDictionary::word(unsigned id)
{
    if(!m_dict.contains(id))
        return Word();
    return m_dict[id];
}

bool MainDictionary::edit(unsigned id, MainDictionary::Word &w)
{
    if(!m_dict.contains(id))
        return false;
    m_dict[id] = w;
    save();
    return true;
}

bool MainDictionary::edit(unsigned id, QString name, QString img, QString boySnd, QString girlSnd)
{
    if(!m_dict.contains(id))
        return false;
    m_dict[id] = Word(name, img, boySnd, girlSnd);
    save();
    return true;
}

QStringList MainDictionary::list()
{
    if(m_dict.isEmpty())
        return QStringList();
    QStringList list;
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        list.append(itr.value().name);
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

QString MainDictionary::wordName(unsigned id)
{
    if(!m_dict.contains(id))
        return QString("");
    return m_dict[id].name;
}

void MainDictionary::save()
{
    m_dictSave.clear();
    for(auto itr = m_dict.begin(); itr != m_dict.end(); ++itr)
    {
        WordSave ws(itr.value());
        m_dictSave[itr.key()] = ws;
    }

    ofstream os(".//editor//mainDict.cereal");
    BinaryOutputArchive archive(os);
    archive(*this);
}

void MainDictionary::load()
{
    m_dictSave.clear();
    m_dict.clear();

    ifstream is(".//editor//mainDict.cereal");
    if(!is.is_open())
    {
        m_id = 0;
        return;
    }
    BinaryInputArchive archive(is);
    archive(*this);
    for(auto itr = m_dictSave.begin(); itr != m_dictSave.end(); ++itr)
    {
        m_dict[itr->first] = Word(itr->second);
    }
}
