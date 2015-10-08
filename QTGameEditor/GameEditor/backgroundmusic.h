#ifndef BACKGROUNDMUSIC_H
#define BACKGROUNDMUSIC_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <QString>
#include <QProcess>
#include <QComboBox>

class BackgroundMusic
{
public:
    BackgroundMusic();
    void add(QString filepath, float vol, float pitch);
    QString remove(QString baseName);
    void play();
    void stop();
    void setVolume(int volume, QString fnBase);
    void setPitch(int pitch, QString fnBase);
    int getVolume(QString fnBase);
    int getPitch(QString fnBase);
    vector<BGM::musicItem> & getMusicData(){return m_music;}
    void setComboBox(QComboBox * x){m_selectBGM = x;}
    void reset();

private:
    int findIndex(QString & fnBase);
    vector<BGM::musicItem> m_music; // Used to store data
    QProcess m_process;
    int m_id;
    QComboBox * m_selectBGM;
};

#endif // BACKGROUNDMUSIC_H
