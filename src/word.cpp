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
