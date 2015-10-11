#include "dictionarysets.h"
#include <QMap>
#include <QSet>
#include <unordered_map>
using std::map;
#include <set>
using std::set;
#include <QString>
#include <QStringList>
#include "maindictionary.h"
#include "choosefromlist.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
using cereal::PortableBinaryInputArchive;
using cereal::PortableBinaryOutputArchive;
#include <fstream>
using std::ofstream;
using std::ifstream;

DictionarySets::DictionarySets()
{
    load();
}

bool DictionarySets::addWord(unsigned dictSetId, unsigned wordId)
{
    if(!m_dictSets.contains(dictSetId))
        return false;
    if(!m_dictSets[dictSetId].ids.contains(wordId))
        return false;
    m_dictSets[dictSetId].ids.insert(wordId);
    save();
    return true;
}

bool DictionarySets::addWord(unsigned dictSetId, QString wordName,
                             MainDictionary *mainDict)
{
    return addWord(dictSetId, mainDict->id(wordName));
}

bool DictionarySets::contains(unsigned dictSetId, QString wordName,
                              MainDictionary *mainDict)
{
    if(!m_dictSets.contains(dictSetId))
        return false;
    return m_dictSets[dictSetId].ids.contains(mainDict->id(wordName));
}

unsigned DictionarySets::getSelection()
{
    QStringList delList;
    QString selection = chooseFromList(this->dictSetsList(), delList, QString("Choose a Dictionary Set")).getSelection();
    for(auto itr = delList.begin(); itr != delList.end(); ++itr)
    {
        this->removeDictSet(*itr);
    }
    save();
    return this->dictSetId(selection);
}

QString DictionarySets::dictSetName(unsigned id)
{
    if(!m_dictSets.contains(id))
        return QString("");
    return m_dictSets[id].name;
}

QStringList DictionarySets::dictSetsList()
{
    QStringList list;
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        list.append(itr.value().name);
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

QStringList DictionarySets::dictSetList(unsigned dictSetId, MainDictionary *mainDict)
{
    if(!m_dictSets.contains(dictSetId))
        return QStringList();
    QStringList list;
    for(auto itr = m_dictSets[dictSetId].ids.begin();
        itr != m_dictSets[dictSetId].ids.end(); ++itr)
    {
        QString wordName = mainDict->wordName(*itr);
        if(!wordName.isEmpty())
            list.append(wordName);
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

DictionarySets::DictSet DictionarySets::dictSet(unsigned dictSetId)
{
    if(!m_dictSets.contains(dictSetId))
        return DictSet();
    return m_dictSets[dictSetId];
}

unsigned DictionarySets::dictSetId(QString dictSetName)
{
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        if(itr.value().name == dictSetName)
            return itr.key();
    }
    return 0;
}

bool DictionarySets::removeWord(unsigned dictSetId, unsigned wordId)
{
    if(!m_dictSets.contains(dictSetId))
        return false;
    if(!m_dictSets[dictSetId].ids.contains(wordId))
        return false;
    m_dictSets[dictSetId].ids.remove(wordId);
    save();
    return true;
}

unsigned DictionarySets::addDictSet(QString dictSetName)
{
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        if(itr.value().name == dictSetName)
            return 0;
    }
    m_dictSets[++m_id] = DictSet(dictSetName);
    save();
    return m_id;
}

unsigned DictionarySets::addDictSet(QString dictSetName, QStringList wordNames,
                                    MainDictionary *mainDict)
{
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        if(itr.value().name == dictSetName)
            return 0;
    }
    QSet<unsigned> wordIds;
    for(auto itr = wordNames.begin(); itr != wordNames.end(); ++itr)
    {
        unsigned id = mainDict->id(*itr);
        if(id != 0)
            wordIds.insert(id);
    }
    m_dictSets[++m_id] = DictSet(dictSetName, wordIds);
    save();
    return m_id;
}

bool DictionarySets::removeDictSet(QString dictSetName)
{
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        if(itr.value().name == dictSetName)
        {
            m_dictSets.remove(itr.key());
            save();
            return true;
        }
    }
    return false;
}

bool DictionarySets::editDictSet(unsigned dictSetId, QString dictSetName,
                                 QStringList wordNames,
                                 MainDictionary *mainDict)
{
    if(!m_dictSets.contains(dictSetId))
        return false;
    if(m_dictSets[dictSetId].name != dictSetName)
    {
        for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
        {
            if(itr.value().name == dictSetName)
                return false;
        }
    }
    QSet<unsigned> wordIds;
    for(auto itr = wordNames.begin(); itr != wordNames.end(); ++itr)
    {
        unsigned id = mainDict->id(*itr);
        if(id != 0)
            wordIds.insert(id);
    }
    m_dictSets[dictSetId] = DictSet(dictSetName, wordIds);
    save();
    return true;
}

void DictionarySets::save()
{
    m_dictSetsSave.clear();
    for(auto itr = m_dictSets.begin(); itr != m_dictSets.end(); ++itr)
    {
        m_dictSetsSave[itr.key()] = DictSetSave(itr.value());
    }
    ofstream os(".//editor//dictSets.cereal", std::ios::binary | std::ios_base::binary | ofstream::out | std::ios::trunc);
    PortableBinaryOutputArchive archive(os);
    archive(*this);
}

void DictionarySets::load()
{
    cout << "dictSet load called" << endl;
    m_dictSets.clear();
    m_dictSetsSave.clear();

    ifstream is(".//editor//dictSets.cereal", std::ios::binary | std::ios_base::binary | ifstream::in);
    if(!is.is_open())
    {
        m_id = 0;
        return;
    }
    PortableBinaryInputArchive archive(is);
    archive(*this);
    for(auto itr = m_dictSetsSave.begin(); itr != m_dictSetsSave.end(); ++itr)
    {
        m_dictSets[itr->first] = DictSet(itr->second);
    }
}
