#ifndef BADGES_H
#define BADGES_H

#include <QMap>
#include <QString>
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::wstring;

class Badges
{
public:
    struct BadgeSave;
    struct Badge{
        Badge():name(QString("")), img(QString("")){}
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
        BadgeSave():name(L""), img(L""){}
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
};

#endif // BADGES_H
