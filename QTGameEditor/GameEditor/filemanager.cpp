#include "filemanager.h"
#include <QString>
#include "myrect.h"
#include "activity.h"
#include "screenqgv.h"
#include "screen.h"
#include "backgroundmusic.h"
#include "map.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <cereal/archives/binary.hpp>
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <QFileInfoList>
#include <vector>
using std::vector;
#include <string>
using std::string;
using std::wstring;
#include <memory>
using std::unique_ptr;
using std::shared_ptr;
#include <QString>
#include <map>
#include <list>
using std::list;
#include <QBitArray>
#include <unordered_map>
using std::unordered_map;
#include "dictionary.h"
#include "scene.h"
#include <qdebug.h>
#include <set>
using std::set;
#include <map>
using std::map;
#include <QMap>
#include <QVector>

#include "mainwindow.h"
#include "ui_mainwindow.h"

class MainWindow;
class Ui_MainWindow;

QMap<FileManager::File, QString> setup()
{
    QMap<FileManager::File, QString> f;
    f[FileManager::ACTBGM] = QString(".//audio//background//minigames//");
    f[FileManager::SCNBGM] = QString(".//audio//background//scenes//");
    f[FileManager::MAPBGM] = QString(".//audio//background//");
    f[FileManager::OTHERSND] = QString(".//audio//other//");
    f[FileManager::FEMSND] = QString(".//audio//words//Female//");
    f[FileManager::MALESND] = QString(".//audio//words//Male//");
    f[FileManager::ACTDIR] = QString(".//db//activities//");
    f[FileManager::MAP] = QString(".//db//maps//WorldMap.map");
    f[FileManager::SCNDIR] = QString(".//db//scenes//");
    f[FileManager::BADGES] = QString(".//editor//badges.cereal");
    f[FileManager::DICTSETS] = QString(".//editor//dictSets.cereal");
    f[FileManager::FILES] = QString(".//editor//files.cereal");
    f[FileManager::MAINDICT] = QString(".//editor//mainDict.cereal");
    f[FileManager::PAIRACTS] = QString(".//editor//pairActs.cereal");
    f[FileManager::MATCHACTS] = QString(".//editor//matchActs.cereal");
    f[FileManager::IMG] = QString(".//images//");
    f[FileManager::WORDIMG] = QString(".//images//words//");
    return f;
}

QMap<FileManager::File, QString> FileManager::files = setup();

FileManager::FileManager()
{
    /* TODO: give creator options
    - delete files (from resources) if they are not referenced
    - don't delete files at all
    - delete files if their references goes to 0
    - Button to deploy (maybe call it clean up)
        - deletes all unnecessary files
        */

    // Make directories if they don't already exist
    QDir dir("editor");
    if (!dir.exists()) {dir.mkpath(".");}

    QFile f(".//editor//files.cereal");
    if(f.exists())
    {
        ifstream ifile(".//editor//files.cereal", std::ios::binary);
        BinaryInputArchive archive(ifile);
        archive(*this);
    }

    vector<QDir> directories = {
        QString(".//audio//"),
        QString(".//audio//background//"),
        QString("./audio//background//minigames//"),
        QString(".//audio//background//scenes//"),
        QString(".//audio//other//"),
        QString(".//audio//words//"),
        QString(".//audio//words//Female//"),
        QString(".//audio//words//Male//"),
        QString(".//images//"),
        QString(".//images//words//"),
        QString(".//db//"),
        QString(".//db//profilepage//"),
        QString(".//db//maps//"),
        QString(".//db//scenes//"),
        QString(".//db//activities//")
    };

    // Make directories if they don't already exist
    for(size_t i = 0; i < directories.size(); ++i)
    {
        if (!directories[i].exists()) {directories[i].mkpath(".");}
    }

    /*
    // Remove files from m_files that are not being referenced
    for(auto itr = m_files.begin(); itr != m_files.end();)
    {
        if(itr->second <= 0)
        {
            itr = m_files.erase(itr);
        }
        else
            ++itr;
    }

    // Delete any files that are not in m_files from directories
    for(int i = 0; i < directories.size(); ++i)
    {
        QFileInfoList list = directories[i].entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        for(auto itr = list.begin(); itr != list.end(); ++itr)
        {
            if(m_files.find(itr->filePath().toStdString()) == m_files.end())
            {
                QFile f(itr->filePath());
                f.remove();
            }
        }
    }
    */
}

QString FileManager::copyFile(QString & filepath, FileManager::File file)
{
    QString newFilepath = files[file];
    newFilepath += QFileInfo(filepath).fileName();
    QFile(filepath).copy(newFilepath);
    ++m_files[newFilepath.toStdString()];
    return newFilepath;
}

#include <qdebug.h>
void FileManager::saveFiles(ScreenQGV &screen, BackgroundMusic & bgm)
{
    vector<ScreenItem> sItems;
    ScreenItem bgi;
    bgi.type = ScreenItemType::IMAGE;
    bgi.name = screen.getBGIFilepath().toStdWString();
    bgi.size = Vec2(screen.sceneRect().width(), screen.sceneRect().height());
    qreal xScale = screen.sceneRect().width()/800.0;
    qreal yScale = screen.sceneRect().height()/600.0;
    sItems.push_back(bgi);
    ScreenItem box;
    box.type = ScreenItemType::BOX;
    box.name = QString("instructionbox").toStdWString();
    box.size = Vec2(600*xScale, 200*yScale);
    box.position = Vec2(100*xScale, 100*yScale);
    box.color = Color(0, 0, 0, 255);
    box.visible = false;
    sItems.push_back(box);
    ScreenItem text;
    text.type = ScreenItemType::TEXT;
    text.name = QString("Help_Text").toStdWString();
    text.size = Vec2(10*xScale, 0*yScale);
    text.position = Vec2(105*xScale, 105*yScale);
    text.visible = false;
    sItems.push_back(text);

    vector<MusicItem> mItems;
    for(auto itr = bgm.getMusicData().begin(); itr != bgm.getMusicData().end(); ++itr)
    {
        MusicItem mus;
        mus.file.assign(itr->filename.begin(), itr->filename.end());
        mus.volume = itr->volume;
        mus.pitch = itr->pitch*2./100.;
        mItems.push_back(mus);
    }

    vector<unique_ptr<MyRect>> * rectItems = screen.rectItems();
    for(auto itr = rectItems->begin(); itr != rectItems->end(); ++itr)
    {
        ScreenItem item;
        item.type = (*itr)->getType();
        item.behavior = 0;
        if((*itr)->mouseDownEngage())
            item.behavior |= MOUSE_DOWN_ENGAGE;
        if((*itr)->mouseDownSelect())
            item.behavior |= MOUSE_DOWN_SELECT;
        if((*itr)->mouseUpEngage())
            item.behavior |= MOUSE_UP_ENGAGE;
        if((*itr)->mouseUpSelect())
            item.behavior |= MOUSE_UP_SELECT;
        if((*itr)->hoverEngage())
            item.behavior |= HOVER_ENGAGE;
        if((*itr)->hoverSelect())
            item.behavior |= HOVER_SELECT;

        QString str;
        if((*itr)->hasSound())
        {
            str = (*itr)->sndFilepath();
            if(str.isEmpty())
                str = QString("none");
            item.sound = str.toStdWString();
            item.volume = (*itr)->volume();
            item.pitch = (*itr)->pitch();
        }
        str = (*itr)->getHoverText();
        if(str.isEmpty())
            str = QString("none");
        item.hover_text = str.toStdWString();
        item.position = Vec2((*itr)->x(), (*itr)->y());
        item.size = Vec2((*itr)->rect().width(), (*itr)->rect().height());
        item.color = Color((*itr)->getColor().red(), (*itr)->getColor().green(),
                           (*itr)->getColor().blue(), (*itr)->getColor().alpha());
        if((*itr)->getType() == ScreenItemType::TEXT)
        {
            str = (*itr)->getText();
            if(str.isEmpty())
                str = QString("none");
            item.name = str.toStdWString();
        }
        else if((*itr)->getType() == ScreenItemType::IMAGE)
        {
            str = (*itr)->imgFilepath();
            if(str.isEmpty())
                str = QString("none");
            item.name = str.toStdWString();
        }
        else // HOTSPOT, BOX
        {
            str = (*itr)->hotspotFilepath();
            if(str.isEmpty())
                str = QString("none");
            item.name = str.toStdWString();
        }
        sItems.push_back(item);
    }

    shared_ptr<Dictionary> dictionary = shared_ptr<Dictionary>(new Dictionary());
    saveDictSet(screen, dictionary);
    if(screen.editorType() == ScreenQGV::MAP)
    {
        string name = "WorldMap";
        Map m(name, dictionary);
        m.setBGM(mItems); // Set Music
        m.setScreenItems(sItems);
        unordered_map<unsigned, string> scenes;
        vector<unique_ptr<MyRect>> * items = screen.rectItems();
        for(size_t i = 0; i < items->size(); ++i)
        {
            if((*items)[i]->id() != 0)
                scenes[i+3] = QString::number((*items)[i]->id()).toStdString(); // + 3 for first 3 items
        }
        m.setScenes(scenes);
        // Badges
        vector<unique_ptr<MyRect>> * rects = screen.rectItems();
        unsigned index = 3;
        for(auto itr = rects->begin(); itr != rects->end(); ++itr)
        {
            set<unsigned> setSave;
            set<wstring> setStrSave;
            QSet<unsigned> aSet = (*itr)->reqBadges();
            for(auto itr2 = aSet.begin(); itr2 != aSet.end(); ++itr2)
            {
                qDebug() << "index:" << (*itr2);
                setSave.insert(*itr2);
                setStrSave.insert(MainWindow::m_badges->badgeName(*itr2).toStdWString());
            }
            m.m_scnPreRegsIds[index] = setSave;
            m.m_scene_prereqs[index] = setStrSave;
            ++index;
        }
        m.save();
    }
    else if(screen.editorType() == ScreenQGV::SCENE)
    {
        string name = QString::number(screen.id()).toStdString();
        Scene s(name, dictionary);
        s.setBGM(mItems);
        s.setScreenItems(sItems);
        vector<MiniGame> acts;
        vector<unique_ptr<MyRect>> * rects = screen.rectItems();
        map<set<unsigned>, unsigned> reqs;
        // Go through MyRect objects to find the activites that the
        // scene links to and push them into acts.
        for(auto itr = rects->begin(); itr != rects->end(); ++itr)
        {
            if(!(*itr)->id())
                continue;
            string actFileName = (*itr)->actFileName().toStdString();
            bool foundit = false;
            for(auto itr2 = acts.begin(); itr2 != acts.end(); ++itr2)
            {
                if(itr2->second == actFileName)
                {
                    foundit = true;
                    break;
                }
            }
            if(!foundit)
            {
                MiniGame mg;
                mg.first = (*itr)->gameType();
                mg.second = actFileName;
                acts.push_back(mg);
            }
        }
        // Go through Activity filenames, create sets which contain
        // indexes into the ScreenItems vector that link to a filename,
        // place into map which maps the set to the index of the filename in
        // the acts vector.
        for(int i = 0; i < acts.size(); ++i)
        {
            set<unsigned> aSet;
            int index = 3;
            for(auto itr2 = rects->begin(); itr2 != rects->end(); ++itr2)
            {
                if(!(*itr2)->id())
                    continue;
                if(acts[i].second == (*itr2)->actFileName().toStdString())
                {
                    aSet.insert(index);
                }
                ++index;
            }
            reqs[aSet] = i;
        }
        s.setRequirements(reqs);
        s.setActivities(acts);
        Badge b;
        if(screen.rewardBadgeId())
        {
            Badges::Badge b2 = MainWindow::m_badges->badge(screen.rewardBadgeId());
            b.m_name = b2.name.toStdWString();
            b.m_image = b2.img.toStdWString();
            unsigned index = 0;
            wstring badgeName = MainWindow::m_badges->badgeName(screen.rewardBadgeId()).toStdWString();
            for(auto itr = rects->begin(); itr != rects->end(); ++itr)
            {
                if((*itr)->id())
                {
                    Piece p;
                    p.m_id = index;
                    ++index;
                    p.m_badge_name = badgeName;
                    p.m_image = (*itr)->actPieceFilepath().toStdWString();
                    b.m_pieces.push_back(p);
                }
            }
            b.m_total_pieces = b.m_pieces.size();
        }
        s.setBadge(b);
        s.save();
    }
    else if(screen.editorType() == ScreenQGV::PAIRACT)
    {
        string name = "p";
        name += QString::number(screen.id()).toStdString();
        Activity act(name, dictionary);
        act.setBGM(mItems);
        act.setScreenItems(sItems);
        act.setChoices(screen.choices());
        act.setRounds(screen.rounds());
        act.setMaxScore(screen.choices());
        act.save();
    }
    else // MATCHACT
    {
        string name = "m";
        name += QString::number(screen.id()).toStdString();
        Activity act(name, dictionary);
        act.setBGM(mItems);
        act.setScreenItems(sItems);
        act.setChoices(screen.choices());
        act.setRounds(screen.rounds());
        act.setMaxScore(screen.rounds());
        act.save();
    }

    //ofstream ofile(".//editor//files.cereal", std::ios::binary);
    //BinaryOutputArchive archive(ofile);
    //archive(*this);
}

void FileManager::loadFiles(ScreenQGV &screen, BackgroundMusic &bgm)
{
    bgm.reset();
    vector<ScreenItem> sItems;
    vector<MusicItem> mItems;

    string name = "WorldMap";
    shared_ptr<Dictionary> dictionary = shared_ptr<Dictionary>(new Dictionary); // Don't need to load in
    Map m(name, dictionary);

    name = QString::number(screen.id()).toStdString();
    Scene s(name, dictionary);
    QMap<unsigned, MiniGame> toActivity;

    name = "p";
    name += QString::number(screen.id()).toStdString();
    Activity pairAct(name, dictionary);

    name = "m";
    name += QString::number(screen.id()).toStdString();
    Activity matchAct(name, dictionary);

    if(screen.editorType() == ScreenQGV::MAP)
    {
        m.load();
        sItems = m.getScreenItems();
        mItems = m.getBGM();
    }
    else if(screen.editorType() == ScreenQGV::SCENE)
    {
        s.load();
        sItems = s.getScreenItems();
        mItems = s.getBGM();
        vector<MiniGame> acts = s.m_activities;
        map<set<unsigned>, unsigned> reqs = s.m_requirements;
        for(auto itr = reqs.begin(); itr != reqs.end(); ++itr)
        {
            for(auto itr2 = itr->first.begin(); itr2 != itr->first.end(); ++itr2)
            {
                toActivity[*itr2] = acts[itr->second];
            }
        }

        vector<unique_ptr<MyRect>> * rects = screen.rectItems();
        for(auto itr = rects->begin(); itr != rects->end(); ++itr)
        {
            if((*itr)->id())
            {
                MiniGame mg;
                mg.first = (*itr)->gameType();
                if(mg.first == GameType::PAIR)
                {
                    mg.second = "p";
                }
                else if(mg.first == GameType::MATCHING)
                {
                    mg.second = "m";
                }
                mg.second += QString::number((*itr)->id()).toStdString();
                acts.push_back(mg);
            }
        }

        // Badge
        screen.setRewardBadgeId(MainWindow::m_badges->id(QString::fromStdWString(s.badge().m_name)));
    }
    else if(screen.editorType() == ScreenQGV::PAIRACT)
    {
        pairAct.load();
        sItems = pairAct.getScreenItems();
        mItems = pairAct.getBGM();
        screen.setChoices(pairAct.getChoices());
        screen.setRounds(pairAct.getRounds());
        screen.setMaxScore(pairAct.getMaxScore());
    }
    else // MATCHACT
    {
        matchAct.load();
        sItems = matchAct.getScreenItems();
        mItems = matchAct.getBGM();
        screen.setChoices(matchAct.getChoices());
        screen.setRounds(matchAct.getRounds());
        screen.setMaxScore(matchAct.getMaxScore());
    }

    if(sItems.size() > 0)
    {
        QString str = QString::fromStdWString(sItems.front().name);
        if(str == QString("none"))
            str.clear();
        screen.loadBGI(str);
        qreal oldW = sItems.front().size.x;
        qreal oldH = sItems.front().size.y;
        qreal newW = screen.scene()->width();
        qreal newH = screen.scene()->height();
        qreal xScale = newW/oldW;
        qreal yScale = newH/oldH;
        int index = 0;
        for(auto itr = sItems.begin(); itr != sItems.end(); ++itr)
        {
            if(itr == sItems.begin())
            {
                ++itr;
                if(itr == sItems.end())
                    break;
                ++itr;
                index = 3;
                continue;
            }
            MyRect * rect = screen.addRect();
            rect->setType(itr->type);
            if(screen.editorType() == ScreenQGV::MAP)
            {
                for(auto itr2 = m.m_scnPreRegsIds[index].begin(); itr2 != m.m_scnPreRegsIds[index].end(); ++itr2)
                {
                    rect->addReqBadge(*itr2);
                }
            }
            else if(screen.editorType() == ScreenQGV::SCENE &&
                    toActivity.find(index) != toActivity.end())
            {
                screen.incrementActs();
                if(toActivity[index].first == GameType::PAIR)
                {
                    rect->setId(QString::fromStdString(toActivity[index].second).remove(0, 1).toInt());
                    rect->setGameType(GameType::PAIR);
                }
                else if(toActivity[index].first == GameType::MATCHING)
                {
                    rect->setId(QString::fromStdString(toActivity[index].second).remove(0, 1).toInt());
                    rect->setGameType(GameType::PAIR);
                }
            }
            ++index;
            if(itr->sound != L"none")
            {
                rect->setSndFilepath(QString::fromStdWString(itr->sound));
                rect->setVol(itr->volume);
                rect->setPitch(itr->pitch);
            }
            str = QString::fromStdWString(itr->hover_text);
            if(str == QString("none"))
                str.clear();
            rect->setHoverText(str);
            rect->setPos(itr->position.x*xScale, itr->position.y*yScale);
            rect->setRect(0, 0, itr->size.x*xScale, itr->size.y*yScale);
            rect->setColor(QColor(itr->color.r, itr->color.g, itr->color.b,
                                  itr->color.a));

            rect->setMouseDownEngage(itr->behavior & MOUSE_DOWN_ENGAGE);
            rect->setMouseDownSelect(itr->behavior & MOUSE_DOWN_SELECT);
            rect->setMouseUpEngage(itr->behavior & MOUSE_UP_ENGAGE);
            rect->setMouseUpSelect(itr->behavior & MOUSE_UP_SELECT);
            rect->setHoverEngage(itr->behavior & HOVER_ENGAGE);
            rect->setHoverSelect(itr->behavior & HOVER_SELECT);

            if(itr->type == ScreenItemType::IMAGE)
            {
                str = QString::fromStdWString(itr->name);
                if(str == QString("none"))
                    str.clear();
                rect->setImage(str);
            }
            else if(itr->type == ScreenItemType::TEXT)
            {
                str = QString::fromStdWString(itr->name);
                if(str == QString("none"))
                    str.clear();
                rect->setText(str);
            }
        }

        for(auto itr = mItems.begin(); itr != mItems.end(); ++itr)
        {
            bgm.add(QString::fromStdWString(itr->file), itr->volume, itr->pitch*100./2.);
        }
    }
    vector<unique_ptr<MyRect>> * items = screen.rectItems();
    if(screen.editorType() == ScreenQGV::MAP)
    {
        for(auto itr = m.m_scenes.begin(); itr != m.m_scenes.end(); ++itr)
        {
            (*items)[itr->first-3]->setId(QString::fromStdString(itr->second).toUInt());
        }
    }
    else if(screen.editorType() == ScreenQGV::SCENE)
    {
        qDebug() << "load scene";
        qDebug() << "p:" << s.badge().m_pieces.size();
        vector<Piece> pieces = s.badge().m_pieces;
        vector<unique_ptr<MyRect>> * rects = screen.rectItems();
        for(auto itr = pieces.begin(); itr != pieces.end(); ++itr)
        {
            qDebug() << "id:" << itr->m_id;
            qDebug() << "path:" << QString::fromStdWString(itr->m_image);
            (*rects)[itr->m_id]->setActPieceFilepath(QString::fromStdWString(itr->m_image));
        }
    }
}

void FileManager::deleteFile(QString file)
{
    if(m_files.find(file.toStdString()) == m_files.end())
        return;
    if(--m_files[file.toStdString()] == 0)
    {
        QFile f(file);
        f.remove();
    }
}

void FileManager::saveDictSet(ScreenQGV &screen, shared_ptr<Dictionary> &dict)
{
    if(!screen.dictSetId())
        return;
    DictionarySets::DictSet dictSet = MainWindow::m_dictSets->dictSet(screen.dictSetId());
    for(auto itr = dictSet.ids.begin(); itr != dictSet.ids.end(); ++itr)
    {
        MainDictionary::Word word = MainWindow::m_mainDict->word(*itr);
        Word w(word.name.toStdWString(), word.boySnd.toStdWString(),
                     word.girlSnd.toStdWString(), word.img.toStdWString());
        dict->addWord(w);
    }
}

QString FileManager::errorImgFilepath()
{
    QString filepath = files[FileManager::IMG];
    filepath += QString("error.png");
    return filepath;
}
