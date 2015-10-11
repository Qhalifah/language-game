#ifndef BACKGROUNDMUSIC_H
#define BACKGROUNDMUSIC_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <QString>
#include "screen.h"
#include <QMediaPlayer>
#include <QComboBox>

class BackgroundMusic
{

public:
    BackgroundMusic();
    void add(QString filepath, float vol, float pitch);
    QString remove(QString baseName);
    void play(QString);
    void stop(QString);
    bool isPlaying(QString);
    void playAll();
    void stopAll();
    void setVolume(int volume, QString fnBase);
    int getVolume(QString fnBase);
    vector<MusicItem> & getMusicData(){return m_music;}
    void setComboBox(QComboBox * x){m_selectBGM = x;}
    void reset();

private:
    int findIndex(QString & fnBase);

    vector<MusicItem> m_music;// background music items
    vector<QMediaPlayer *> m_media_players;// background media players
    int m_id;
    QComboBox * m_selectBGM;
};

#endif // BACKGROUNDMUSIC_H
