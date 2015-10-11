#ifndef MAINDICTIONARY_H
#define MAINDICTIONARY_H

#include <QMap>
#include <QString>
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::wstring;
#include <QDialog>
#include <iostream>
using std::cout;
using std::endl;
#include "cereal/types/unordered_map.hpp"

class MainDictionary
{
public:
    struct WordSave;
    struct Word{
        Word():name(QString("")), img(QString("")),
            boySnd(QString("")), girlSnd(QString("")){}
        Word(QString & n, QString & i, QString & b, QString & g):
            name(n), img(i), boySnd(b), girlSnd(g){}
        Word(WordSave & w):name(QString::fromStdWString(w.name)),
            img(QString::fromStdWString(w.img)),
            boySnd(QString::fromStdWString(w.boySnd)),
            girlSnd(QString::fromStdWString(w.girlSnd)){}
        void clear(){
            name.clear(); img.clear(); boySnd.clear(); girlSnd.clear();
        }

        QString name;
        QString img;
        QString boySnd;
        QString girlSnd;
    };
    struct WordSave{
        WordSave():name(L""), img(L""), boySnd(L""), girlSnd(L""){}
        WordSave(Word & w): name(w.name.toStdWString()),
            img(w.img.toStdWString()), boySnd(w.boySnd.toStdWString()),
            girlSnd(w.girlSnd.toStdWString()){}
        wstring name;
        wstring img;
        wstring boySnd;
        wstring girlSnd;
        template <class Archive>
        void serialize(Archive & ar)
        {
            ar(name, boySnd, girlSnd, img);
        }
    };

    MainDictionary();
    unsigned add(Word & w);
    unsigned add(QString name, QString img, QString boySnd, QString girlSnd);
    bool remove(unsigned id);
    unsigned id(QString name);
    Word word(QString name);
    Word word(unsigned id);
    bool edit(unsigned id, Word & w);
    bool edit(unsigned id, QString name, QString img, QString boySnd, QString girlSnd);
    QStringList list();
    QString wordName(unsigned id);

    template <class Archive>
    void serialize(Archive & ar)
    {
        cout << "About to archive" << endl;
        ar(m_dictSave, m_id);
    }

protected:
    void save();
    void load();

    QMap<unsigned, Word> m_dict;
    unordered_map<unsigned, WordSave> m_dictSave;
    unsigned m_id;
};

#endif // MAINDICTIONARY_H
