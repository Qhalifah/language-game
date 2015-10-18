#ifndef DBMAP_H
#define DBMAP_H

#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
using std::wstring;
#include <QString>
#include <QMap>
#include <QStringList>
#include <windows.h>
#include <iostream>
#include <shlobj.h>


class DBMap
{
public:
    DBMap(string filename);
    ~DBMap();

    unsigned add(QString name); // Returns false if scene name already exists
    QString getSelection(); // User chooses scene name to edit/delete from QDialog
    bool rename(unsigned id, QString name);
    void rename(QString oldName, QString newName);
    QStringList list(); // Returns all scene names
    unsigned id(QString name);
    QString name(unsigned id);

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(
            m_id,
            m_dbMapSave
          );
    }

private:
    void remove(QString name); // Removes the scene with that name
    void save();
    void load(string filename);
    QMap<unsigned, QString> m_dbMap; // Unique id to name of scene
    unordered_map<unsigned, string> m_dbMapSave; // Needed for saving/loading using cereal
    unsigned m_id; // An id for the next scene to be added, increments for each use
    wchar_t cStr[MAX_PATH]; // Used to fill m_DocumentsPath
    std::string m_DocumentsPath; // Used to get the user's Document file path
};

#endif // DBMAP_H
