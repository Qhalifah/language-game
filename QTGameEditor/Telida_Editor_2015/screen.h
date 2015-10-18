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

#include <SFML/System/Vector2.hpp>
#include <initializer_list>
using std::initializer_list;
using sf::Vector2f;
using sf::Vector2i;
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
#include <windows.h>
#include <iostream>
#include <shlobj.h>

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
class Vec2
{
public:
    Vec2(){}
    Vec2(float t_x, float t_y):x(t_x), y(t_y){}
    Vec2(const Vec2 & orig)
    {
        this->x=orig.x;
        this->y=orig.y;
    }

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                x, y
        );
    }
    float x,y;
};


// Wrapper class to make Color cerealizable.
namespace mine {
class Color
{
public:
    Color(){}
    //Color(initializer_list<uint8_t> inits);
    Color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a = 255):
        r(t_r), g(t_g), b(t_b), a(t_a){}
    /*Color(const sf::Color& init);

    Color operator=(const sf::Color& init)
    {
        this->r = init.r;
        this->g = init.g;
        this->b = init.b;
        this->a = init.a;

        return *this;
    }*/

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
                r, g, b, a
        );
    }
    uint8_t r, g, b, a;
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
    Vec2   position = {0,0};
    Vec2   size     = {0,0};
    float  volume   = 100;
    float  pitch    = 1;
    bool   visible  = true;
    bool   engaged  = false;
    bool   selected = false;
    bool   hover    = false;
    Color  color    =  Color(255, 255, 255, 255);
    uint8_t behavior = 0;

    template<class Archive>
    void serialize(Archive & ar)
    {
     // Size down TEXT objects so they are the correct size in the game editor
        if(type == TEXT)
        {
            size.x /= name.size();
        }

        ar(
                type,
                name,
                sound,
                hover_text,
                position,
                size,
                volume,
                pitch,
                visible,
                color,
                behavior
        );

        // Size up TEXT objects so they are the correct size in the game editor
        if(type == TEXT)
        {
            size.x *= name.size();
        }
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
};



class Screen
{
public:
    Screen(shared_ptr<Dictionary> dictionary)
    : m_screenItems(), m_BGM(), m_dictionary(dictionary)
    {
        // Data members will be initialized to default values, and
        // derived class objects will populate with data.

        // Get the user's Documents folder path
        SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, DWORD("FOLDERID_Documents"), cStr);
        m_DocumentsPath = QString::fromStdWString(cStr).toStdString();
    }
    virtual ~Screen(){}

    const vector<ScreenItem>& getScreenItems() const
    {
        return m_screenItems;
    }
    const vector<MusicItem>&  getBGM() const
    {
        return m_BGM;
    }
    //const shared_ptr<Dictionary>& getDictionary() const;

    void setBGM(vector<MusicItem>& BGM){m_BGM = BGM;}
    void setScreenItems(vector<ScreenItem>& screenItems){m_screenItems=screenItems;}
    //void setScreenItem(unsigned index, Vec2 position, Vec2 size)
    //void setScreenItem(unsigned index, const ScreenItem& new_val);
    //void setDictionary(shared_ptr<Dictionary> dictionary)

protected:

    vector<ScreenItem> m_screenItems;   // Interactibles/soundeffects
    vector<MusicItem>  m_BGM;           // Background music filenames
    shared_ptr<Dictionary> m_dictionary;
    std::string m_DocumentsPath;

private:

    wchar_t cStr[MAX_PATH];
};
#endif /* SCREEN_H_ */
