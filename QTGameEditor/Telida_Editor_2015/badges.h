#ifndef BADGES_H
#define BADGES_H

#include <QMap>
#include <QString>
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::wstring;
#include <windows.h>
#include <iostream>
#include <shlobj.h>

class Badges
{
public:
    struct BadgeSave;
    struct Badge{
        Badge():name(QString("none")), img(QString("none")){}
        Badge(QString & n, QString & i):
            name(n), img(i){}
        Badge(BadgeSave & b):name(QString::fromStdWString(b.name)),
            img(QString::fromStdWString(b.img)){}
        void clear(){
            name.clear(); img.clear();
        }

        QString name;
        QString img;
    };
    struct BadgeSave{
        BadgeSave():name(L"none"), img(L"none"){}
        BadgeSave(Badge & b): name(b.name.toStdWString()),
            img(b.img.toStdWString()){}
        wstring name;
        wstring img;
        template <class Archive>
        void serialize(Archive & ar)
        {
            ar(name, img);
        }
    };

    Badges();
    unsigned add(Badge & b);
    bool remove(unsigned id);
    unsigned id(QString name);
    Badge badge(QString name);
    Badge badge(unsigned id);
    bool edit(unsigned id, Badge & b);
    QStringList list();
    QString badgeName(unsigned id);
    unsigned getSelection();

    template <class Archive>
    void serialize(Archive & ar)
    {
        ar(m_badgesSave, m_id);
    }

protected:
    void save();
    void load();

    QMap<unsigned, Badge> m_badges;
    unordered_map<unsigned, BadgeSave> m_badgesSave;
    unsigned m_id;

private:
    wchar_t cStr[MAX_PATH];
    std::string m_DocumentsPath;
};

#endif // BADGES_H
