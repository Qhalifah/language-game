/*Copywrite 2014 Shaun Bond, Jeffery Waldon, Charlie Carlson, Simon Smith, Joseph DeWilde, Aaron Andrews*/

/*This file is part of Dina'ki Adventures.

Dina'ki Adventures is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dina'ki Adventures is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dina'ki Adventures. If not, see <http://www.gnu.org/licenses/>.*/

//Jeffrey Waldron
//2/11/2014
//Dictionary.h

//This is the cpp file for the Dictionary class
//A Dictionary is just a bunch of words (see word.h)

#include "dictionary.h"
#include <random>
#include <chrono>


Dictionary::Dictionary()
{
    //Done. No need to add things to a blank Dictionary
	dictSet_id = "None";
}

//TODO
Dictionary::Dictionary(std::string const& fileLocation)
{
	dictSet_id = "None";
}

//TODO
Dictionary::~Dictionary()
{

}

void Dictionary::addWord(Word & given)
{
    m_words[given.getDinName()] = given;
}


void Dictionary::loadDictionary(std::string const& fileLocation)
{
    std::ifstream is(fileLocation);
    cereal::BinaryInputArchive archive(is);
    archive(*this);
}

void Dictionary::saveDictionary(std::string const& fileLocation)
{
    std::ofstream os(fileLocation);
    cereal::BinaryOutputArchive archive(os);
    archive(*this);
}

Word Dictionary::operator[](std::wstring given)
{
    auto findMe = m_words.find(given);
    if (findMe == m_words.end())
    {
        //If you can't find it, send an empty word
        return Word();
    }
    else
    {
        return findMe->second;
    }
}


Word Dictionary::getRandomWord()
{
    auto itr = m_words.begin();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator (seed); // mt19937 is a standard //mersenne_twister_engine
    unsigned int temp = generator();
    //temp = abs(temp);
    temp %= m_words.size();

    for(unsigned int i = 0; i != temp; i++)
    {
        itr ++;
    }

    return itr->second;
}

unsigned int Dictionary::getSize()
{
    return m_words.size();
}

void Dictionary::addInputWord()
{
    //NOTE ON FLAGS:
    //Flags were added encase additional functionality about checking the input is to be added
    bool flag = true;
    std::wstring dinName;
    std::wstring boyLoc;
    std::wstring girlLoc;
    std::wstring imageLoc;
    while (flag)
    {
        std::wcout <<"\nPlease enter the Dinake name for a word: ";
        std::getline(std::wcin, dinName);
        if (flag)
        {
            flag = false;
        }
    }

    flag = true;
    while (flag)
    {
        std::wcout <<"\nPlease enter the Boy audio location for a word: ";
        std::getline(std::wcin, boyLoc);
        if (flag)
        {
            flag = false;
        }
    }

    flag = true;
    while (flag)
    {
        std::wcout <<"\nPlease enter the Girl audio location for a word: ";
        std::getline(std::wcin, girlLoc);
        if (flag)
        {
            flag = false;
        }
    }

    flag = true;
    while (flag)
    {
        std::wcout <<"\nPlease enter the Image location for a word: ";
        std::getline(std::wcin, imageLoc);
        if (flag)
        {
            flag = false;
        }
    }
    Word addMe(dinName, boyLoc, girlLoc, imageLoc);
    addWord(addMe);

}

void Dictionary::printWords()
{
    std::wcout <<"Being printing Words\n";
    std::wcout <<"\n-------------------------------------------------";
    for(auto i = m_words.begin(); i != m_words.end(); i++)
    {
        std::wcout <<"\nDinaki Name:         " << i->second.getDinName();
        std::wcout <<"\nBoy Audio Location:  " << i->second.getBoyAudioLocation();
        std::wcout <<"\nGirl Audio Location: " << i->second.getGirlAudioLocation();
        std::wcout <<"\nImage Location:      " << i->second.getImageLocation();
        std::wcout <<"\n-------------------------------------------------";
    }
}
