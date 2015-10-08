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
// pitch should be between [0, 100]
void BackgroundMusic::add(QString filepath, float vol, float pitch)
{
    BGM::musicItem m = {filepath.toStdString(), ++m_id, vol, pitch};
    m_music.push_back(m);
    m_com.m_id = m.id;
    m_com.m_filename = m.filename;
    m_com.m_vol = m.volume;
    m_com.m_pitch = m.pitch;
    m_com.m_action = Communication::ADD;
    while(!m_com.saveToFile(".\\Com.txt"));
    m_process.putChar('c');

    QString base = QFileInfo(filepath).baseName();
    if(m_selectBGM->findText(base) < 0) // Add if it's not already added
        m_selectBGM->addItem(base);
}

QString BackgroundMusic::remove(QString baseName)
{
    int index = findIndex(baseName);
    QString filepath = QString((m_music[index].filename).c_str());
    m_com.m_action = Communication::REMOVE;
    m_com.m_id = m_music[index].id;
    m_music.erase(m_music.begin()+index);
    m_com.saveToFile(".\\Com.txt");
    m_process.putChar('c');
    return filepath;
}

void BackgroundMusic::play()
{
    if(m_music.empty())
        return;
    m_bgm.m_mus = m_music;
    m_process.start(QString(".\\SFML_BGM"));
}

void BackgroundMusic::stop()
{
    m_com.m_stop = true;
    while(!m_com.saveToFile(".\\Com.txt"));
    m_com.m_stop = false;
    m_process.putChar('c');
}

void BackgroundMusic::setVolume(int volume, QString fnBase)
{
    int index = findIndex(fnBase);
    m_music[index].volume = volume;
    m_com.m_id = m_music[index].id;
    m_com.m_action = Communication::VOLUME;
    m_com.m_filename = m_music[index].filename;
    m_com.m_vol = volume;
    m_com.m_pitch = m_music[index].pitch;
    m_com.m_stop = false;
    while(!m_com.saveToFile(".\\Com.txt"));
    m_process.putChar('c');
}

void BackgroundMusic::setPitch(int pitch, QString fnBase)
{
    int index = findIndex(fnBase);
    m_music[index].pitch = pitch;
    m_com.m_id = m_music[index].id;
    m_com.m_action = Communication::PITCH;
    m_com.m_filename = m_music[index].filename;
    m_com.m_vol = m_music[index].volume;
    m_com.m_pitch = pitch;
    m_com.m_stop = false;
    while(!m_com.saveToFile(".\\Com.txt"));
    m_process.putChar('c');
}

int BackgroundMusic::getVolume(QString fnBase)
{
    int index = findIndex(fnBase);
    return m_music[index].volume;
}

int BackgroundMusic::getPitch(QString fnBase)
{
    int index = findIndex(fnBase);
    return m_music[index].pitch;
}

void BackgroundMusic::reset()
{
    this->stop();
    m_music.clear();
    QString temp = m_selectBGM->itemText(0);
    m_selectBGM->clear();
    m_selectBGM->addItem(temp);
}

int BackgroundMusic::findIndex(QString & fnBase)
{
    for(int i = 0; i < m_music.size(); ++i)
    {
        if(fnBase == QFileInfo(QString(m_music[i].filename.c_str())).baseName())
        {
            return i;
        }
    }
    return -1;
}
