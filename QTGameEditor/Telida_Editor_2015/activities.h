#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
#include <QString>
#include <QMap>
#include <QStringList>
#include <windows.h>
#include <iostream>
#include <shlobj.h>

class Activities
{
public:
    Activities(QString filename);
    struct ActSave;
    struct Activity{
        Activity():dictSetId(0), name(QString("none")){}
        Activity(ActSave a):dictSetId(a.dictSetId),
            name(QString::fromStdString(a.name)){}
        unsigned dictSetId;
        QString name;
    };
    struct ActSave{
        ActSave():dictSetId(0), name("none"){}
        ActSave(Activity a):dictSetId(a.dictSetId),
            name(a.name.toStdString()){}
        unsigned dictSetId;
        string name;
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar(name, dictSetId);
        }
    };

    unsigned add(Activity act); // Returns false if activity name already exists
    QString getSelection(); // User chooses activity name to edit/delete from QDialog
    bool edit(unsigned id, Activity act);
    QStringList list(); // Returns all activity names
    unsigned id(QString name);
    Activity act(unsigned id);
    QStringList listWithName(QString name);

    template <class Archive>
    void serialize( Archive & ar )
    {
        cout << "Archiving begin" << endl;
        cout << "Archiving m_id: " << m_id << endl;
        ar(
            m_id,
            m_actSave
          );
    }
    bool remove(QString name); // Removes the scene with that name
private:

    void save();
    void load();
    QString m_filename;
    QMap<unsigned, Activity> m_acts; // Unique id to name of scene
    unordered_map<unsigned, ActSave> m_actSave; // Needed for saving/loading using cereal
    unsigned m_id; // An id for the next activity to be added, increments for each use
    wchar_t cStr[MAX_PATH];
    std::string m_DocumentsPath;
};

#endif // ACTIVITIES_H
