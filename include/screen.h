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

// screen.h
// Shaun Bond
// 3/20/2014
//
// This class is intended to be used as a base class for Maps, Scenes, and
// Activities.  It has some common data members, and some common functionality.
// It is not intended to be instantiated as a separate object and each type of
// Screen has different requirements for constructing its sprites, so the
// Screen class does not initialize any of the data members.

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Vector2i;
#include <initializer_list>
using std::initializer_list;
#include <utility>
using std::pair;
#include <string>
using std::wstring;
#include <vector>
using std::vector;
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <iostream>
#include <memory>
using std::shared_ptr;
#include "dictionary.h"
#include <iostream>
using std::cout;
using std::endl;

// Data type to represent possible types of screen items.
enum ScreenItemType
{
    IMAGE,
    TEXT,
    BOX,
    BUTTON,
    HOTSPOT
};

enum Behavior
{
    MOUSE_DOWN_ENGAGE = 1,
    MOUSE_DOWN_SELECT = 2,
    MOUSE_UP_ENGAGE = 4,
    MOUSE_UP_SELECT = 8,
    HOVER_ENGAGE = 16,
    HOVER_SELECT = 32
};


// Wrapper class to make Vector2f cerealizable.
class Vec2 : public Vector2f
{
public:
    Vec2();
    Vec2(initializer_list<float> inits);
    Vec2(float t_x, float t_y);
    Vec2(const Vec2& init);

    Vec2 operator=(const Vec2& init)
    {
        this->x = init.x;
        this->y = init.y;

        return *this;
    }

    Vec2 operator+(const Vec2& rhs)
    {
        Vec2 fu(this->x + rhs.x, this->y + rhs.y);
        return fu;
    }

    Vec2 operator*(const Vector2f& rhs) const
    {
        Vec2 fu(this->x * rhs.x, this->y * rhs.y);
        return fu;
    }

    Vec2 operator*(const double rhs) const
    {
        Vec2 fu(this->x * rhs, this->y * rhs);
        return fu;
    }

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                x, y
        );
    }
};


// Wrapper class to make Color cerealizable.
namespace mine {
class Color : public sf::Color
{
public:
    Color();
    Color(initializer_list<uint8_t> inits);
    Color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a = 255);
    Color(const sf::Color& init);

    Color operator=(const sf::Color& init)
    {
        this->r = init.r;
        this->g = init.g;
        this->b = init.b;
        this->a = init.a;

        return *this;
    }

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                r, g, b, a
        );
    }
};
}
using mine::Color;


// Data structure to contain information about objects for rendering.
// If type == IMAGE, then image contains filename.
// If type == TEXT, then image contains text to be display.
// If type == IMAGE, then size is same as image size.
//   otherwise, size is the size of the hotspot or text item.
// Sound is associated sound's filename.
// Volume and pitch are sound settings.
struct ScreenItem
{
    ScreenItemType type = IMAGE;
    wstring name    = L"none";
    wstring sound   = L"none";
    wstring hover_text = L"none";
    Vec2   position = Vec2{0,0};
    Vec2   size     = Vec2{0,0};
    float  volume   = 100;
    float  pitch    = 1;
    bool   visible  = true;
    bool   engaged  = false;
    bool   selected = false;
    bool   hover    = false;
    Color  color    = Color::White;
    uint8_t behavior = 0;

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                type,
                name,
                sound,
                hover_text,
                position,
                size,
                volume,
                pitch,
                visible, // TODO: is this a good idea?
                color,
                behavior
        );
    }

    bool operator==(const ScreenItem& rval) const;
};



// Data structure to contain information about background music
// associated with the screen.
struct MusicItem
{
    wstring file  = L"none";
    float volume = 40;
    float pitch  = 1;

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                file,
                volume,
                pitch
        );
    }

    bool operator==(const MusicItem& rval) const;
};



class Screen
{
public:
    Screen(shared_ptr<Dictionary> dictionary);
    virtual ~Screen();

    const vector<ScreenItem>& getScreenItems() const;
    const ScreenItem getScreenItem(size_t index) const;
    const vector<MusicItem>&  getBGM() const;
    const shared_ptr<Dictionary>& getDictionary() const;

    void setBGM(vector<MusicItem>& BGM);
    void setScreenItems(vector<ScreenItem>& screenItems);
    void setScreenItem(unsigned index, Vec2 position, Vec2 size);
    void setScreenItem(unsigned index, const ScreenItem& new_val);
    void setDictionary(shared_ptr<Dictionary> dictionary);

    virtual void save() = 0;
    virtual void load() = 0;

protected:

    vector<ScreenItem> m_screenItems;   // Interactibles/soundeffects
    vector<MusicItem>  m_BGM;           // Background music filenames
    shared_ptr<Dictionary> m_dictionary;


private:

};
#endif /* SCREEN_H_ */
