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

bool Dictionary::removeWord(std::wstring wordName)
{
    auto itr = m_words.find(wordName);
    if(itr == m_words.end())
            return false;
    m_words.erase(wordName);
    return true;
}

void Dictionary::loadDictionary(std::string const& fileLocation)
{
    cout << "dict load called" << endl;
    std::ifstream is(fileLocation, std::ios::binary);
    cereal::BinaryInputArchive archive(is);
    archive(*this);
}

void Dictionary::saveDictionary(std::string const& fileLocation)
{
    std::ofstream os(fileLocation, std::ios::binary);
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
    cout << "Dictionary " << dictSet_id << endl;
    cout << "Dictionary size is: " << m_words.size() << endl;

    if(m_words.size() > 0)
    {
        std::cout <<"Begin printing Words\n";
        std::cout <<"\n-------------------------------------------------" << endl;
        for(auto i = m_words.begin(); i != m_words.end(); i++)
        {
            std::cout <<"\nDinaki Name:         " << QString::fromStdWString(i->second.getDinName()).toStdString();
            std::cout <<"\nBoy Audio Location:  " << QString::fromStdWString(i->second.getBoyAudioLocation()).toStdString();
            std::cout <<"\nGirl Audio Location: " << QString::fromStdWString(i->second.getGirlAudioLocation()).toStdString();
            std::cout <<"\nImage Location:      " << QString::fromStdWString(i->second.getImageLocation()).toStdString();
            std::cout <<"\n-------------------------------------------------" << endl;
        }
    }
}

void Dictionary::setDictSetID(std::string newID)
{
    dictSet_id = newID;
}

std::string Dictionary::getDictSetID()
{
    return dictSet_id;
}
