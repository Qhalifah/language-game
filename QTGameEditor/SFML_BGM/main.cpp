#include <iostream>
using std::cout;
using std::endl;
#include "BGM.h"
#include <SFML/Audio/Music.hpp>
using sf::Music;
#include <memory>
using std::move;
using std::unique_ptr;
#include "Communication.h"
#include <string>
using std::string;
#include <map>
using std::map;
#include <utility>
using std::pair;
#include <cstdio>

int main()
{
    BGM dat;
    if(!dat.loadFromFile(".\\BGM.cereal"))
    {
        cout << "No BGM file." << endl;
        return 0;
    }
    Communication c;
    if(!c.loadFromFile(".\\Com.txt"))
    {
        cout << "No com file." << endl;
        return 0;
    }

    map<int, unique_ptr<Music>> BGmus;
    for(int i = 0; i < dat.m_mus.size(); ++i)
    {
        auto ret = BGmus.insert(pair<int, unique_ptr<Music>>
                                (dat.m_mus[i].id, unique_ptr<Music>(new Music())));
        if(!BGmus[dat.m_mus[i].id]->openFromFile(dat.m_mus[i].filename))
            cout << "Could not open " << dat.m_mus[i].filename << endl;
        ret.first->second->setVolume(dat.m_mus[i].volume);
        ret.first->second->setPitch(dat.m_mus[i].pitch*2./100.);
        ret.first->second->setLoop(true);
        ret.first->second->play();
    }
    while(!c.m_stop)
    {
        getchar();
        if(c.loadFromFile(".\\Com.txt"))
        {
            switch(c.m_action)
            {
            case Communication::VOLUME:
                if(BGmus.find(c.m_id) != BGmus.end())
                    BGmus[c.m_id]->setVolume(c.m_vol);
                break;
            case Communication::PITCH:
                if(BGmus.find(c.m_id) != BGmus.end())
                    BGmus[c.m_id]->setPitch(c.m_pitch*2./100.);
                break;
            case Communication::ADD:
                {
                    auto ret = BGmus.insert(pair<int, unique_ptr<Music>>
                                            (c.m_id, unique_ptr<Music>(new Music())));
                    ret.first->second->openFromFile(c.m_filename);
                    ret.first->second->setVolume(c.m_vol);
                    ret.first->second->setPitch(c.m_pitch*2./100.);
                    ret.first->second->setLoop(true);
                    ret.first->second->play();
                    c.m_action = Communication::NONE;
                    c.saveToFile(".\\Com.txt");
                    break;
                }
            case Communication::REMOVE:
                {
                    auto itr = BGmus.find(c.m_id);
                    itr->second->stop();
                    BGmus.erase(itr);
                    c.m_action = Communication::NONE;
                    c.saveToFile(".\\Com.txt");
                    break;
                }
                default:
                    break;
            };
        }
    }
    return 0;
}
