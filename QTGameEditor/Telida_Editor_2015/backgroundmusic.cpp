#include "backgroundmusic.h"
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <QFileInfo>
#include <QString>
#include <QProcess>
#include <QThread>

BackgroundMusic::BackgroundMusic():m_id(0)
{
}

// volume should be between [0, 100]
// pitch should be between [1, 10]
void BackgroundMusic::add(QString filepath, float vol, float pitch)
{
    QString base = QFileInfo(filepath).baseName();

    if(m_selectBGM->findText(base) < 0) // Add if it's not already added
    {
        m_selectBGM->addItem(base);

        MusicItem newMusicItem;
        newMusicItem.file = filepath.toStdWString();
        newMusicItem.volume = vol;
        newMusicItem.pitch = 1;
        m_music.push_back(newMusicItem);

        m_media_players.push_back(new QMediaPlayer);
    }
}

QString BackgroundMusic::remove(QString baseName)
{
    auto jj = m_media_players.begin();
    for(auto ii=m_music.begin(); ii!=m_music.end(); ++ii, ++jj)
    {
        if(QFileInfo(QString::fromStdWString(ii->file)).baseName() == baseName)
        {
            m_music.erase(ii);
            m_media_players.erase(jj);
            break;
        }
    }
    return "Select Background Music";
}

void BackgroundMusic::play(QString baseName)
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            m_media_players[ii]->setVolume(m_music[ii].volume);
            m_media_players[ii]->setMedia(QUrl::fromLocalFile(QString::fromStdWString(m_music[ii].file)));
            m_media_players[ii]->play();
            break;
        }
    }
}

void BackgroundMusic::stop(QString baseName)
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            m_media_players[ii]->stop();
            break;
        }
    }
}

bool BackgroundMusic::isPlaying(QString baseName)
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            if(m_media_players[ii]->state() == QMediaPlayer::PlayingState)
                return true;
            else
                return false;
        }
    }
    return false;
}

void BackgroundMusic::playAll()
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        m_media_players[ii]->setVolume(m_music[ii].volume);
        m_media_players[ii]->setMedia(QUrl::fromLocalFile(QString::fromStdWString(m_music[ii].file)));
        m_media_players[ii]->play();
    }
}

void BackgroundMusic::stopAll()
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        m_media_players[ii]->stop();
    }
}

void BackgroundMusic::setVolume(int volume, QString baseName)
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            m_music[ii].volume = volume;
            m_media_players[ii]->setVolume(volume);
            break;
        }
    }
}

int BackgroundMusic::getVolume(QString baseName)
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            return m_music[ii].volume;
        }
    }

    return 0;
}

void BackgroundMusic::reset()
{
    for(int ii=0; ii<m_music.size(); ++ii)
    {
        m_media_players[ii]->stop();
    }
    m_music.clear();
    m_media_players.clear();

    m_selectBGM->clear();
    m_selectBGM->addItem(QString::fromStdString("Select Background Music"));
}

int BackgroundMusic::findIndex(QString & baseName)
{
    for(int ii = 0; ii < m_music.size(); ++ii)
    {
        if(QFileInfo(QString::fromStdWString(m_music[ii].file)).baseName() == baseName)
        {
            return ii;
        }
    }
    return -1;
}
