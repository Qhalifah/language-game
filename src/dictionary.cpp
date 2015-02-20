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
}

//TODO
Dictionary::Dictionary(std::string const& fileLocation)
{

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
