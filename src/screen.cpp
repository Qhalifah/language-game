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

// screen.cpp
// Shaun Bond
// 3/5/2014
//
// This is the implementation for the Screen class.

#include "screen.h"

Vec2::Vec2()
: Vector2f() {}

Vec2::Vec2(initializer_list<float> inits)
: Vector2f(*inits.begin(),*(inits.begin()+1)) {}

Vec2::Vec2(float t_x, float t_y)
: Vector2f(t_x, t_y) {}

Vec2::Vec2(const Vec2& init)
: Vector2f(init.x, init.y){}

mine::Color::Color()
: sf::Color() {}

mine::Color::Color(initializer_list<uint8_t> inits)
: sf::Color(*inits.begin(), *(inits.begin()+1), *(inits.begin()+2), 255) {}

mine::Color::Color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a)
: sf::Color(t_r, t_g, t_b, t_a){}

mine::Color::Color(const sf::Color& init)
: sf::Color(init) {}

bool ScreenItem::operator==(const ScreenItem& rval) const
{
    return (this->type == rval.type &&
            this->name == rval.name &&
            this->sound == rval.sound &&
            this->position == rval.position &&
            this->size == rval.size &&
            this->volume == rval.volume &&
            this->pitch == rval.pitch);
}

bool MusicItem::operator==(const MusicItem& rval) const
{
    return (this->file == rval.file &&
            this->volume == rval.volume &&
            this->pitch == rval.pitch);
}

Screen::Screen(shared_ptr<Dictionary> dictionary)
: m_screenItems(), m_BGM(), m_dictionary(dictionary)
{
	// Data members will be initialized to default values, and
    // derived class objects will populate with data.

	// Get the user's Documents folder path
	wchar_t cStr[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, DWORD("FOLDERID_Documents"), cStr);
	m_DocumentsPath = sf::String(cStr).toAnsiString();
}

Screen::~Screen()
{
}

const vector<MusicItem>& Screen::getBGM() const
{
    return m_BGM;
}

const vector<ScreenItem>& Screen::getScreenItems() const
{
    return m_screenItems;
}

const ScreenItem Screen::getScreenItem(size_t index) const
{
    return m_screenItems[index];
}

const shared_ptr<Dictionary>& Screen::getDictionary() const
{
    return m_dictionary;
}

void Screen::setBGM(vector<MusicItem>& BGM)
{
    swap(m_BGM, BGM);
}

void Screen::setScreenItems(vector<ScreenItem>& screenItems)
{
    swap(m_screenItems, screenItems);
}

void Screen::setScreenItem(unsigned index, Vec2 position, Vec2 size)
{
    m_screenItems[index].position = position;
    m_screenItems[index].size = size;
}

void Screen::setScreenItem(unsigned index, const ScreenItem& new_val)
{
    m_screenItems[index] = new_val;
}

void Screen::setDictionary(shared_ptr<Dictionary> dictionary)
{
    m_dictionary = dictionary;
}
