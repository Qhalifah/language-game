#ifndef DICTIONARYSETS_H
#define DICTIONARYSETS_H

#include <QMap>
#include <QSet>
#include <QString>
#include <unordered_map>
using std::unordered_map;
#include <set>
using std::set;
#include <string>
using std::wstring;
#include "maindictionary.h"
#include <QVector>

class DictionarySets
{
public:
    struct DictSetSave;
    struct DictSet{
        DictSet():name(QString("")){}
        DictSet(QString n):name(n){}
        DictSet(QString n, QSet<unsigned> i):name(n), ids(i){}
        DictSet(DictSetSave & s):name(QString::fromStdWString(s.name)){
            for(auto itr = s.ids.begin(); itr != s.ids.end(); ++itr)
            {
                ids.insert(*itr);
            }
        }

        QString name; // name of dictionary set
        QSet<unsigned> ids; // id to Word in main dictionary
    };
    struct DictSetSave{
        DictSetSave():name(L""){}
        DictSetSave(DictSet & s):name(s.name.toStdWString()){
            for(auto itr = s.ids.begin(); itr != s.ids.end(); ++itr)
            {
                ids.insert(*itr);
            }
        }

        wstring name;
        set<unsigned> ids;
        template <class Archive>
        void serialize(Archive & ar)
        {
            ar(name, ids);
        }
    };

    DictionarySets();

    // addWord
    // Will return false if dictionary set does not exist
    // or if word is already added. User should give valid
    // dict set id and assume any false is due to an already
    // added word.
    bool addWord(unsigned dictSetId, unsigned wordId);
    bool addWord(unsigned dictSetId, QString wordName, MainDictionary * mainDict);
    bool contains(unsigned dictSetId, QString wordName, MainDictionary * mainDict);
    unsigned getSelection();
    QString dictSetName(unsigned id);
    QStringList dictSetsList(); // List of dictionary set names
    // dictSetList
    // List of words in a dictionary set
    QStringList dictSetList(unsigned dictSetId, MainDictionary * mainDict);
    DictSet dictSet(unsigned dictSetId);
    unsigned dictSetId(QString dictSetName);
    bool removeWord(unsigned dictSetId, unsigned wordId);
    unsigned addDictSet(QString dictSetName);
    unsigned addDictSet(QString dictSetName, QStringList wordNames,
                        MainDictionary * mainDict);
    bool removeDictSet(QString dictSetName);
    bool editDictSet(unsigned dictSetId, QString dictSetName,
                     QStringList wordNames, MainDictionary * mainDict);

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(m_dictSetsSave, m_id);
    }

protected:
    void save();
    void load();
    QMap<unsigned, DictSet> m_dictSets;
    unordered_map<unsigned, DictSetSave> m_dictSetsSave;
    unsigned m_id;
};

#endif // DICTIONARYSETS_H
