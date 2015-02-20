#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include "myrect.h"
#include "screenqgv.h"
#include "backgroundmusic.h"
#include <map>
using std::map;
#include <string>
using std::string;
using std::wstring;
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <QMap>
#include "dictionary.h"
#include <map>
#include <list>
using std::list;
#include "maindictionary.h"
#include "dictionarysets.h"
#include "activities.h"

class FileManager{
public:
    enum File {ACTBGM, SCNBGM, MAPBGM, OTHERSND, FEMSND, MALESND, // Audio
              ACTDIR, MAP, SCNDIR,    // database
              BADGES, DICTSETS, FILES, MAINDICT, PAIRACTS, MATCHACTS, // editor
              IMG, WORDIMG, // images
              };
    static QMap<File, QString> files;


    struct WordItem{
        WordItem():word(QString("")), imgFilepath(QString("")),
            maleVoiceFilepath(QString("")), femVoiceFilepath(QString(""))
        {}
        WordItem(QString w, QString img, QString mV, QString fV):
            word(w), imgFilepath(img), maleVoiceFilepath(mV),
            femVoiceFilepath(fV)
            {}
        void set(QString & w, QString & img, QString & fem, QString & mal)
        {
            word = w;
            imgFilepath = img;
            femVoiceFilepath = fem;
            maleVoiceFilepath = mal;
        }
        void clear()
        {
            word = QString("");
            imgFilepath = QString("");
            femVoiceFilepath = QString("");
            maleVoiceFilepath = QString("");
        }

        QString word;
        QString imgFilepath;
        QString maleVoiceFilepath;
        QString femVoiceFilepath;
    };

    FileManager();
    QString copyFile(QString & filepath,
                  File file);

    void saveFiles(ScreenQGV & screen, BackgroundMusic & bgm);
    void loadFiles(ScreenQGV &screen, BackgroundMusic & bgm);
    void deleteFile(QString file);
    void saveDictSet(ScreenQGV & screen, shared_ptr<Dictionary> & dict);
    static QString errorImgFilepath();

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(m_files);
    }
private:
    map<string, int> m_files;
};

#endif // FILEMANAGER_H
