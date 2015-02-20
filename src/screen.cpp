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
	// Do nothing.  Data members will be initialized to default values, and
    // derived class objects will populate with data.
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
