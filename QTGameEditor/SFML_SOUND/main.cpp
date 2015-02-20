#include <iostream>
using std::cout;
using std::endl;
#include <SFML/Audio/Sound.hpp>
using sf::Sound;
#include <SFML/Audio/SoundBuffer.hpp>
using sf::SoundBuffer;
#include <memory>
using std::move;
using std::unique_ptr;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <cstdio>

bool load(string filename, float & vol, float & pitch, string & soundPath)
{
    ifstream ifile(filename);
    if(!ifile.is_open())
    {
        cout << "Could not open file to save: " << filename << endl;
        return false;
    }

    int playSound;
    string line = "";
    ifile >> playSound;
    if(!playSound)
        return false;
    getline(ifile, line);
    ifile >> vol;
    getline(ifile, line);
    ifile >> pitch;
    getline(ifile, line);
    getline(ifile, soundPath);
    ifile.close();
    return true;
}

bool save(string filename, float & vol, float & pitch, string & soundPath)
{
    ofstream ofile(filename);
    if(!ofile.is_open())
    {
        cout << "Could not open file to save: " << filename << endl;
        return false;
    }
    int playSound = false;
    ofile << playSound;
    ofile << "\n";
    ofile << vol;
    ofile << "\n";
    ofile << pitch;
    ofile << "\n";
    ofile << soundPath;
    ofile.close();
    return true;
}

int main()
{
    string soundPath = "";
    float volume = 50;
    float pitch = 50;
    unique_ptr<Sound> snd;
    unique_ptr<SoundBuffer> sndBuf;
    while(true)
    {
        getchar();
        if(load(".\\ComSnd.txt", volume, pitch, soundPath))
        {
            snd.reset();
            sndBuf.reset();
            snd = unique_ptr<Sound>(new Sound);
            sndBuf = unique_ptr<SoundBuffer>(new SoundBuffer);
            if(!sndBuf->loadFromFile(soundPath))
                cout << "Could not load from file " << soundPath << endl;
            snd->setBuffer(*sndBuf);
            snd->setVolume(volume);
            snd->setPitch(pitch*2./100.);
            snd->play();
            while(!save(".\\ComSnd.txt", volume, pitch, soundPath));
        }
    }
    return 0;
}
