//Jeffrey Waldron
//2/11/2014
//Word.cpp

//This is the cpp for a Word Class
//A Word is a word in both ENG and DIN as well as locations to the DIN prononceation and image

#include "word.h"

Word::Word()
{
    m_dinName = L"EMPTY";
    m_boyAudioLocation = L"EMPTY";
    m_girlAudioLocation = L"EMPTY";
    m_imageLocation = L"EMPTY";
}


Word::Word(std::wstring const & dinName,
           std::wstring const & boyAudioLocation,
           std::wstring const & girlAudioLocation,
           std::wstring const & imageLocation)
{
    m_dinName = dinName;
    m_boyAudioLocation = boyAudioLocation;
    m_girlAudioLocation = girlAudioLocation;
    m_imageLocation = imageLocation;
}

Word::~Word()
{

}

std::wstring Word::getDinName()
{
    return m_dinName;
}

std::wstring Word::getBoyAudioLocation()
{
    return m_boyAudioLocation;
}

std::wstring Word::getGirlAudioLocation()
{
    return m_girlAudioLocation;
}
std::wstring Word::getImageLocation()
{
    return m_imageLocation;
}
