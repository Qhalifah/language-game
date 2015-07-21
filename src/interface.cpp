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

// interface.cpp
// Shaun Bond
// 3/5/2014
//
//

#include "interface.h"
using sf::Image;
using sf::RectangleShape;
using sf::IntRect;
using sf::FloatRect;
#include <iostream>
using std::wcerr;
using std::wcout;
using std::endl;
#include <tuple>
using std::pair;


void Interface::loadError(const wstring& file, const wstring& problem, const wstring& func)
{
    wcerr << "Error: " << problem << " '" << file
         << "' in class Interface: function '" << func << "'"
         << endl;
}

// Load a character image from disk into character map for rendering.
void Interface::loadCharacter(wchar_t chr, const string& file)
{
    m_font_textures.push_back(make_shared<Texture>());

    if( !m_font_textures.back()->loadFromFile(file) )
    {
        std::cerr << "Failed to load: " << file <<  std::endl;
        m_font_textures.back()->loadFromFile("images/error.png");
    }

    m_font_textures.back()->setSmooth(true);
    m_font.insert(pair<wchar_t, Sprite>(chr, Sprite(*m_font_textures.back())));
}

// TODO: need to know hud and background size at construction to properly size
// Right now hackishly hard coding. Resize function properly sizes hud.
Interface::Interface()
: m_hud_width(m_win_w),
  m_hud_height(m_win_w * .1),
  m_hud_scale(1),
  m_scale(1, 1),
  m_font(64),
  m_window(new RenderWindow(sf::VideoMode(m_win_w, m_win_h+m_hud_height),
                                          "Dinak'i Adventures!")),
  m_view(m_window->getDefaultView())
{
    // Higlighting effect for focused objects implemented as shader object
    if(!m_focusshader.loadFromFile("shaders/focus_f.glsl", Shader::Fragment))
    {
        loadError(L"focus shaders", L"couldn't load", L"Interface::render()");
    }

    // Raised button effect for buttons
    if(!m_normalshader.loadFromFile("shaders/normal_f.glsl", Shader::Fragment))
    {
        loadError(L"normal shaders", L"couldn't load", L"Interface::render()");
    }

    // Raised button effect for buttons
    if(!m_fontshader.loadFromFile("shaders/font_f.glsl", Shader::Fragment))
    {
        loadError(L"font shaders", L"couldn't load", L"Interface::render()");
    }

    // Constants for use in focusshader.
    // Each row is for a different ScreenItem type.
    // Each col is for a different constant.
    const unsigned char constants[20] = { 255,   0,   0,   0,
                                          0,   255,   0,   0,
                                          255,   0,   0,   0,
                                          0,     0, 255,   0,
                                          0,     0,   0, 255 };
    konstants.create(5,1);
    konstants.update(constants);

    m_focusshader.setParameter("konstants", konstants);
    m_focusshader.setParameter("tex1", sf::Shader::CurrentTexture);
    m_normalshader.setParameter("konstants", konstants);
    m_normalshader.setParameter("tex1", sf::Shader::CurrentTexture);
    m_fontshader.setParameter("tex1", sf::Shader::CurrentTexture);

    // Load character map with sprites for drawing.
    loadCharacter('A', "./font/ca.png");
    loadCharacter('a', "./font/la.png");
    loadCharacter('B', "./font/cb.png");
    loadCharacter('b', "./font/lb.png");
    loadCharacter('C', "./font/cc.png");
    loadCharacter('c', "./font/lc.png");
    loadCharacter('D', "./font/cd.png");
    loadCharacter('d', "./font/ld.png");
    loadCharacter('E', "./font/ce.png");
    loadCharacter('e', "./font/le.png");
    loadCharacter('F', "./font/cf.png");
    loadCharacter('f', "./font/lf.png");
    loadCharacter('G', "./font/cg.png");
    loadCharacter('g', "./font/lg.png");
    loadCharacter('H', "./font/ch.png");
    loadCharacter('h', "./font/lh.png");
    loadCharacter('I', "./font/ci.png");
    loadCharacter('i', "./font/li.png");
    loadCharacter('J', "./font/cj.png");
    loadCharacter('j', "./font/lj.png");
    loadCharacter('K', "./font/ck.png");
    loadCharacter('k', "./font/lk.png");
    loadCharacter('L', "./font/cl.png");
    loadCharacter('l', "./font/ll.png");
    loadCharacter('M', "./font/cm.png");
    loadCharacter('m', "./font/lm.png");
    loadCharacter('N', "./font/cn.png");
    loadCharacter('n', "./font/ln.png");
    loadCharacter('O', "./font/co.png");
    loadCharacter('o', "./font/lo.png");
    loadCharacter('P', "./font/cp.png");
    loadCharacter('p', "./font/lp.png");
    loadCharacter('Q', "./font/cq.png");
    loadCharacter('q', "./font/lq.png");
    loadCharacter('R', "./font/cr.png");
    loadCharacter('r', "./font/lr.png");
    loadCharacter('S', "./font/cs.png");
    loadCharacter('s', "./font/ls.png");
    loadCharacter('T', "./font/ct.png");
    loadCharacter('t', "./font/lt.png");
    loadCharacter('U', "./font/cu.png");
    loadCharacter('u', "./font/lu.png");
    loadCharacter('V', "./font/cv.png");
    loadCharacter('v', "./font/lv.png");
    loadCharacter('W', "./font/cw.png");
    loadCharacter('w', "./font/lw.png");
    loadCharacter('X', "./font/cx.png");
    loadCharacter('x', "./font/lx.png");
    loadCharacter('Y', "./font/cy.png");
    loadCharacter('y', "./font/ly.png");
    loadCharacter('Z', "./font/cz.png");
    loadCharacter('z', "./font/lz.png");
    loadCharacter(L'\u0141', "./font/clws.png"); // L with stroke
    loadCharacter(L'\u0142', "./font/llws.png"); // l with stroke
    loadCharacter(L'\'', "./font/apos.png");     // Apostrophe
    loadCharacter(L'\u02cb', "./font/apos.png"); // Apostrophe
    loadCharacter('?', "./font/question.png");
    loadCharacter('.', "./font/period.png");
    loadCharacter(',', "./font/comma.png");
    loadCharacter('!', "./font/exclam.png");
    loadCharacter(':', "./font/colon.png");
    loadCharacter(' ', "./font/space.png");
    loadCharacter('0', "./font/0.png");
    loadCharacter('1', "./font/1.png");
    loadCharacter('2', "./font/2.png");
    loadCharacter('3', "./font/3.png");
    loadCharacter('4', "./font/4.png");
    loadCharacter('5', "./font/5.png");
    loadCharacter('6', "./font/6.png");
    loadCharacter('7', "./font/7.png");
    loadCharacter('8', "./font/8.png");
    loadCharacter('9', "./font/9.png");
    loadCharacter('-', "./font/minus.png");
    loadCharacter('+', "./font/plus.png");
	loadCharacter('<', "./font/Arrow_Up.png");
	loadCharacter('>', "./font/Arrow_Down.png");
}

Interface::~Interface()
{}

bool Interface::isCharSupported(wchar_t chr)
{
    return m_font.count(chr);
}

const shared_ptr<sf::RenderWindow> Interface::getWindow() const
{
    return m_window;
}

// Takes x, y position and returns index of
// object which is hit or -1 if none.
// Only checks objects which are in current Screen.
int Interface::getScreenHit(float x, float y)
{
    for(int sprite = (int)m_screenSprites.size()-1; sprite >= 0; --sprite )
    {
        if(m_screenItems[(size_t)sprite].visible == true &&
           m_screenSprites[(size_t)sprite]->getGlobalBounds().contains(x, y))
            return sprite;
    }
    return -1;
}

// Takes x, y position and returns index of
// object which is hit or -1 if none.
// Only checks objects which are in the HUD.
int Interface::getHudHit(float x, float y)
{
    for( int sprite = (int)m_hudSprites.size()-1; sprite >= 0; --sprite )
    {
        if(m_hudItems[(size_t)sprite].visible &&
           m_hudSprites[(size_t)sprite]->getGlobalBounds().contains(x, y))
            return sprite;
    }
    return -1;
}

void Interface::playHudSound(size_t index)
{
    if( unsigned(index) < m_hudSounds.size() )
    {
        m_hudSounds[index]->play();
    }
}

void Interface::playScreenSound(size_t index)
{
    if( unsigned(index) < m_screenSounds.size() )
    {
        m_screenSounds[index]->play();
    }
}

void Interface::playBGM(size_t index)
{
    if( unsigned(index) < m_BGM.size() )
    {
        if(m_BGM[index]->getStatus() == sf::Music::Stopped ||
           m_BGM[index]->getStatus() == sf::Music::Paused)
           m_BGM[index]->play();
    }
}

void Interface::playAllBGM()
{
	for (int ii = 0; ii < m_BGM.size(); ++ii)
	{
		if (m_BGM[ii]->getStatus() == sf::Music::Stopped ||
			m_BGM[ii]->getStatus() == sf::Music::Paused)
			m_BGM[ii]->play();
	}
}

void Interface::stopBGM(size_t index)
{
    if( unsigned(index) < m_BGM.size() )
    {
        m_BGM[index]->pause();
    }
}

void Interface::stopAllBGM()
{
	for (int ii = 0; ii < m_BGM.size(); ++ii)
	{
		m_BGM[ii]->pause();
	}
}

void Interface::resizeSprite(shared_ptr<Sprite> sprite,
                             float scale, float h_adjust)
{
    float x = sprite->getPosition().x*scale + h_adjust;
    float y = sprite->getPosition().y*scale;
    sprite->scale(scale, scale);
    sprite->setPosition(x, y);
}

// This function is called by controller upon window resize
void Interface::resize()
{
    float win_h = m_window->getSize().y;
    float win_w = m_window->getSize().x;
    float scale = 1;
    m_view = View(FloatRect(0.f, 0.f, win_w, win_h));
    m_window->setView(m_view);

    if( !m_screenSprites.empty() )
    {
        FloatRect bg = m_screenSprites[0]->getGlobalBounds();

        if( (win_h/win_w) < ((bg.height+m_hud_height)/bg.width) )
        {// Height is short dimension
            scale = win_h / (bg.height+m_hud_height);
        }
        else
        {// Width is short dimension
            scale = win_w / bg.width;
        }

        m_scale *= scale;
        bg.width *= scale;
        bg.left  *= scale;
        float h_adjust = .5 * (win_w-bg.width) - bg.left;

        for( auto sprite : m_screenSprites )
        {
            resizeSprite(sprite, scale, h_adjust);
        }
        for( auto sprite : m_hudSprites )
        {
            resizeSprite(sprite, scale, h_adjust);
        }
        for( auto sprite : m_text )
        {
            resizeSprite(sprite.second, scale, h_adjust);
        }
    }

    m_hud_width = m_hud_width*scale;
    m_hud_height = m_hud_height*scale; //new_hud_h;
    m_hud_scale *= scale;
    m_win_h = win_h;
    m_win_w = win_w;

    render();
}

// This function processes new ScreenItems and creates sprite and sound objects.
void Interface::newHudObject(const ScreenItem& data, size_t index)
{
    wcout << data.name << endl
          << data.sound << endl
          << data.hover_text << endl << endl;

    m_hudTextures.push_back(make_shared<Texture>());
    m_hudSprites.push_back(makeSpriteFromScreenItem(
                                        m_hudTextures.back(),
                                        data));
    m_hudSprites.back()->setPosition(data.position);
    if(data.type == BOX)
    {
        m_hudSprites.back()->move(-borderSize, -borderSize);
    }

    if( !m_soundBuffers.count(data.sound) ) // If sound buffer does not exist
    {
        // Make a new sound buffer
        m_soundBuffers.insert(pair<wstring, shared_ptr<SoundBuffer>>(
                              data.sound, make_shared<SoundBuffer>()));

        if( data.sound != L"none" ) // Try to load sound from file
            if( !m_soundBuffers[data.sound]->loadFromFile(sf::String(data.sound)) )
                std::wcerr << "loading sound failed: " << data.sound << std::endl;
    }

    // Make sound using sound buffer
    m_hudSounds.push_back(make_shared<Sound>());
    m_hudSounds.back()->setBuffer(*m_soundBuffers[data.sound]);
    m_hudSounds.back()->setVolume(data.volume);
    m_hudSounds.back()->setPitch(data.pitch);

    if( data.hover_text != L"none" && data.hover )
    {
        auto texture = makeWordTexture(data.hover_text, {20, 0},
                                       Color(220,150,0));
        m_hudtextTextures.insert(pair< int, shared_ptr<Texture> >
                             (index, texture));
        m_hudtext.insert(pair<int, shared_ptr<Sprite>>
                     (index, make_shared<Sprite>(*texture)));

        // Position sprite relative to edge of screen, scaled with map size
        m_hudtext[index]->setPosition(data.position);

        // Position sprite relative to map
        float bg_width = m_screenSprites[0]->getGlobalBounds().width;
        m_hudtext[index]->move(.5 * (m_window->getSize().x - bg_width), 0);

        m_hudtext[index]->scale(m_scale);
    }
    else
    {
        m_hudtextTextures.erase(index);
        m_hudtext.erase(index);
    }
}

// This function processes new ScreenItems and creates sprite and sound objects.
void Interface::newScreenObject(const ScreenItem& data, shared_ptr<Sound> sound,
                                size_t index)
{
    wcout << data.name << endl
          << data.sound << endl
          << data.hover_text << endl << endl;

    //TODO: Try with empty shared ptr
    m_screenTextures.push_back(make_shared<Texture>());
    m_screenSprites.push_back(makeSpriteFromScreenItem( m_screenTextures.back(),
                                                        data ));

    // Keep sprite proportional to background size.
    m_screenSprites.back()->scale(m_scale);

    positionSprite(m_screenSprites.back(), data);

    if( !m_soundBuffers.count(data.sound) ) // If sound buffer does not exist
    {
        // Make a new sound buffer
        m_soundBuffers.insert(pair<wstring, shared_ptr<SoundBuffer>>(
                              data.sound, make_shared<SoundBuffer>()));

        if( data.sound != L"none" ) // Try to load sound from file
            if( !m_soundBuffers[data.sound]->loadFromFile(sf::String(data.sound)) )
                std::wcerr << "loading sound failed: " << data.sound << std::endl;
    }

    // Make sound using sound buffer
    sound->setBuffer(*m_soundBuffers[data.sound]);
    sound->setVolume(data.volume);
    sound->setPitch(data.pitch);

    // TODO: Same as below
    if( data.hover_text != L"none" && data.hover )
    {
        auto texture = makeWordTexture(data.hover_text, {20, 0},
                                       Color(220,150,0));
        m_textTextures.insert(pair< int, shared_ptr<Texture> >
                             (index, texture));
        m_text.insert(pair< int, shared_ptr<Sprite> >
                     (index, make_shared<Sprite>(*texture)));
        positionSprite(m_text[index], data);
        m_text[index]->scale(m_scale);
    }
}

// This function is called by master controller to inform of change to HUD.
void Interface::updateHudSprite(size_t index, const ScreenItem& data)
{
    if( unsigned(index) < m_hudSprites.size() )
    {
        if( m_hudItems[index].name != data.name )
        {
            // TODO: This is hacky. Need to find a better way to handle hud sizing
            m_hudItems[index] = data;
            m_hudItems[index].size *= m_hud_scale;
            m_hudItems[index].position *= m_hud_scale;
            m_hudSprites[index] = makeSpriteFromScreenItem(m_hudTextures[index],
                                                           m_hudItems[index]);

            // Position sprite relative to edge of screen, scaled with map size
            m_hudSprites[index]->setPosition(m_hudItems[index].position);

            // Position sprite relative to map
            float bg_width = m_screenSprites[0]->getGlobalBounds().width;
            m_hudSprites[index]->move(.5 * (m_window->getSize().x - bg_width), 0);

            // Adjust for border if it is a box
            if(m_hudItems[index].type == BOX)
            {
                m_hudSprites[index]->move(-borderSize, -borderSize);
            }
        }
        else
        { // Same image, but position or size may be different.
            if( data.position != m_hudItems[index].position )
            {
                Vector2f position = m_hud_scale * data.position ;

                // Position sprite relative to edge of screen, scaled with map size
                m_hudSprites[index]->setPosition(position);

                // Position sprite relative to map
                float bg_width = m_screenSprites[0]->getGlobalBounds().width;
                m_hudSprites[index]->move(.5 * (m_window->getSize().x - bg_width), 0);

                // TODO: should check for change of type as well as name
                // Adjust for border if it is a box
                if(m_hudItems[index].type == BOX)
                {
                    m_hudSprites[index]->move(-borderSize, -borderSize);
                }
            }
            if( data.size != m_hudItems[index].size )
            {
                //Vector2f size = m_hud_scale * data.size;

                // TODO: it appears we don't need this.
//                m_hudSprites[index]->scale
//                    (size.x/m_hudItems[index].size.x,
//                     size.y/m_hudItems[index].size.y);
            }
        }

        // TODO: Might want to think about refactoring this
//        if( data.hover != m_hudItems[index].hover ||
//            data.hover_text != m_hudItems[index].hover_text)
        { // Mouse over status has changed.
            if( data.hover_text != L"none" && data.hover )
            {
                auto texture = makeWordTexture(data.hover_text, {20, 0},
                                               Color(220,150,0));
                m_hudtextTextures.insert(pair< int, shared_ptr<Texture> >
                                     (index, texture));
                m_hudtext.insert(pair<int, shared_ptr<Sprite>>
                             (index, make_shared<Sprite>(*texture)));

                // Position sprite relative to edge of screen, scaled with map size
                m_hudtext[index]->setPosition(m_hud_scale * data.position);

                // Position sprite relative to map
                float bg_width = m_screenSprites[0]->getGlobalBounds().width;
                m_hudtext[index]->move(.5 * (m_window->getSize().x - bg_width), 0);

                m_hudtext[index]->scale(m_scale);
            }
            else
            {
                m_hudtextTextures.erase(index);
                m_hudtext.erase(index);
            }
        }

        if( m_hudItems[index].sound != data.sound )
        {
            if( !m_soundBuffers.count(data.sound) ) // If sound buffer does not exist
            {
                // Make a new sound buffer
                m_soundBuffers.insert(pair<wstring, shared_ptr<SoundBuffer>>(
                                      data.sound, make_shared<SoundBuffer>()));

                if( data.sound != L"none" ) // Try to load sound from file
                    if( !m_soundBuffers[data.sound]->loadFromFile(sf::String(data.sound)) )
                        std::wcerr << "loading sound failed: " << data.sound << std::endl;
            }

            // Make sound using sound buffer
            m_hudSounds[index] = make_shared<Sound>();
            m_hudSounds[index]->setBuffer(*m_soundBuffers[data.sound]);
            m_hudSounds[index]->setVolume(data.volume);
            m_hudSounds[index]->setPitch(data.pitch);
        }

        // Save for personal use
        m_hudItems[index] = data;
        m_hudItems[index].size *= m_hud_scale;
        m_hudItems[index].position *= m_hud_scale;
    }
    // TODO: Handle addition of new HUD objects?
    else
    {
        wcerr << "Error: Index too large when updating sprite "
              << data.name << endl;
    }
}

void Interface::updateSprite(size_t index, const ScreenItem& data)
{
    if( unsigned(index) < m_screenSprites.size() )
    {
        if( data.name != m_screenItems[index].name )
        { // Completely new image, make new sprite and position.
            m_screenSprites[index] = makeSpriteFromScreenItem(m_screenTextures[index],
                                                              data);
            m_screenSprites[index]->scale(m_scale);
            positionSprite(m_screenSprites[index], data);
        }
        else
        { // Same image, but position or size may be different.
            if( data.position != m_screenItems[index].position )
            {
                positionSprite(m_screenSprites[index],
                               data);
            }
            if( data.size != m_screenItems[index].size )
            {
                m_screenSprites[index]->scale
                    (data.size.x/m_screenItems[index].size.x,
                     data.size.y/m_screenItems[index].size.y);
            }
        }

        // TODO: Might want to think about refactoring this
        if( data.hover != m_screenItems[index].hover )
        { // Mouse over status has changed.
            if( data.hover_text != L"none" && data.hover )
            {
                auto texture = makeWordTexture(data.hover_text, {20, 0},
                                               Color(220,150,0));
                m_textTextures.insert(pair< int, shared_ptr<Texture> >
                                     (index, texture));
                m_text.insert(pair<int, shared_ptr<Sprite>>
                             (index, make_shared<Sprite>(*texture)));
                positionSprite(m_text[index], data);
                m_text[index]->scale(m_scale);
            }
            else
            {
                m_textTextures.erase(index);
                m_text.erase(index);
            }
        }

        if( data.sound != m_screenItems[index].sound )
        { // Sound has changed.
            if( !m_soundBuffers.count(data.sound) )
            {
               m_soundBuffers.insert(pair<wstring, shared_ptr<SoundBuffer>>
                                    (data.sound, make_shared<SoundBuffer>()));

               if( data.sound != L"none" )
                   if(!m_soundBuffers[data.sound]->loadFromFile(sf::String(data.sound)))
                       wcerr << "Failed to load " << data.sound << endl;
            }

            m_screenSounds[index] = make_shared<Sound>();
            m_screenSounds[index]->setBuffer(*m_soundBuffers[data.sound]);
            m_screenSounds[index]->setVolume(data.volume);
            m_screenSounds[index]->setPitch(data.pitch);
        }

        // Copy new info for later reference.
        m_screenItems[index] = data;
    }
    else if( unsigned(index) == m_screenSprites.size() )
    {// Make a new sprite
        // TODO: test this
        m_screenItems.push_back(data);

        m_screenSounds.push_back(make_shared<Sound>());
        newScreenObject(m_screenItems.back(), m_screenSounds.back(), index);
    }
    else
    {
        wcerr << "Error: Index too large when updating sprite "
                  << data.name.c_str() << endl;
    }
}

// This is a range based version of updateHud
// Takes and index to the first element to update, and one past the last
void Interface::updateHud(size_t first, size_t last, const vector<ScreenItem>& screenItems)
{
    for(auto index = first; index < last; ++index)
    {
        updateHudSprite(index, screenItems[index]);
    }

    render();
}

// This function is called by models to inform of a change to the HUD
void Interface::updateHud(size_t index, const ScreenItem& data)
{
    updateHudSprite(index, data);
    render();
}

// This is a range based version of update
// Takes and index to the first element to update, and one past the last
void Interface::update(size_t first, size_t last, const vector<ScreenItem>& screenItems)
{
    for(auto index = first; index < last; ++index)
    {
        updateSprite(index, screenItems[index]);
    }

    render();
}

// This function is called by models to inform of a change to the screen
void Interface::update(size_t index, const ScreenItem& data)
{
    updateSprite(index, data);
    render();
}

// This function is called by the models to inform of a change to the screen
void Interface::update(size_t index, const MusicItem& data)
{
    if( unsigned(index) < m_BGM.size() )
    {
        // Stop currently playing music
        m_BGM[index]->stop();

        // Create music objects for new screen and start playing them
        m_BGM[index]->openFromFile(sf::String(data.file));
        m_BGM[index]->setVolume(data.volume);
        m_BGM[index]->setPitch(data.pitch);
        m_BGM[index]->setLoop(true);
        m_BGM[index]->play();
    }
}


void Interface::changeHUD(const shared_ptr<HUD> hud)
{
    float win_h = m_window->getSize().y;
    float win_w = m_window->getSize().x;
    win_h -= m_hud_height;
    m_hudItems = hud->getItems();
    unsigned numButtons = m_hudItems.size();
    m_hud_scale = m_hud_width / hud->width();
    m_hud_height = hud->height() * m_hud_scale;
    win_h += m_hud_height;
    m_window->setSize(sf::Vector2u(win_w, win_h));
    m_win_h = win_h;

    m_hudTextures.clear();
    m_hudSprites.clear();
    m_hudSounds.clear();

    for(unsigned ii = 0; ii < numButtons; ++ii)
    {
        m_hudItems[ii].size *= m_hud_scale;
        m_hudItems[ii].position *= m_hud_scale;

        newHudObject(m_hudItems[ii], ii);
    }

    render();
}

void Interface::changeMode(shared_ptr<Screen> new_screen)
{
    m_screenItems = new_screen->getScreenItems();

    // Handle case where there are no images.
    if( m_screenItems.empty() )
    {
        m_screenItems.push_back(ScreenItem());
        m_screenItems[0].type = IMAGE;
        m_screenItems[0].name = L"images/error.png";
        m_screenItems[0].size = {800, 600};
    }

    // Set scale factor from background image size.
    const ScreenItem& bg = m_screenItems[0]; // Background image
    float win_h  = m_window->getSize().y;
    float win_w  = m_window->getSize().x;
    m_view = View(FloatRect(0.f, 0.f, win_w, win_h));
    m_window->setView(m_view);
    win_h = win_h - m_hud_height;

    if( (win_h / win_w) < bg.size.y/bg.size.x ) // Height is short dimension
        m_scale.x = m_scale.y = win_h / bg.size.y;
    else // Width is short dimension
        m_scale.x = m_scale.y = win_w / bg.size.x;

    // Clear sprite and texture data for reconstruction.
    m_screenSprites.clear();
    m_screenTextures.clear();
    m_text.clear();

    // Construct sprites needed for the new screen.
    // Temporary sound vectors, so currently playing sounds can keep doing so.
    vector<shared_ptr<Sound>>       screenSounds;
    for(size_t ii = 0; ii < m_screenItems.size(); ++ii)
    {
        screenSounds.push_back(make_shared<Sound>());
        newScreenObject(m_screenItems[ii], screenSounds.back(), ii);
    }

    render();

    // Create music objects for new screen and start playing them
    vector<shared_ptr<Music>> new_BGM;
    for(auto musicItem : new_screen->getBGM())
    {
        shared_ptr<Music> bgm = make_shared<Music>();
        bgm->openFromFile(sf::String(musicItem.file));
        bgm->setVolume(musicItem.volume);
        bgm->setPitch(musicItem.pitch);
        bgm->setLoop(true);
        bgm->play();
        new_BGM.push_back(bgm);
    }

    // Stop any currently playing music
    for(auto music : m_BGM)
    {
        music->stop();
    }

    m_BGM.clear();

    std::swap(m_BGM, new_BGM);

    bool still_playing = true;
    while(still_playing)
    {
        still_playing = false;
        for(auto sound : m_screenSounds)
        {
            if(sound->getStatus() == Sound::Playing)
                still_playing = true;
        }
    }

    std::swap(m_screenSounds, screenSounds);
}


void Interface::positionSprite(shared_ptr<Sprite> sprite,
                               const ScreenItem& screenItem)
{
    // Position sprite relative to edge of screen, scaled with map size
    sprite->setPosition(screenItem.position*m_scale);

    // Position sprite relative to map
    float bg_width = m_screenSprites[0]->getGlobalBounds().width;
    sprite->move(.5 * (m_window->getSize().x - bg_width), m_hud_height);

    // Adjust for border if it is a box
    if(screenItem.type == BOX)
    {
        sprite->move(-borderSize, -borderSize);
    }
}

// Builds a sprite, and stores it for later display.
// Must pass in a pointer to a texture which already exists.
shared_ptr<Sprite> Interface::makeSprite(shared_ptr<Texture>& texture,
                                         const ScreenItem& screenItem)
{
    texture->setSmooth(true);

    shared_ptr<Sprite> sprite(new Sprite);
    sprite->setTexture(*texture);
    float x = screenItem.size.x/sprite->getTexture()->getSize().x;
    float y = screenItem.size.y/sprite->getTexture()->getSize().y;
    sprite->setScale(x, y); // Size sprite to user desired size.

    return sprite;
}

// Makes a sprite from a ScreenItem
// Must pass in a pointer to a texture which already exists.
// TODO: refactor to make more compact and readable
shared_ptr<Sprite> Interface::makeSpriteFromScreenItem(
                              shared_ptr<Texture>& texture,
                              const ScreenItem& screenItem)
{
    shared_ptr<Sprite> sprite;
    switch(screenItem.type)
    {
    case IMAGE:
    {
        if( screenItem.name != L"none" &&
            !texture->loadFromFile(sf::String(screenItem.name)) )
        {
            texture->loadFromFile("images/error.png");
            loadError(sf::String(screenItem.name),
                      L"Loading texture from file",
                      L"makeSpriteFromScreenItem()");

        }
        else if(screenItem.name == L"none" )
        {
            texture = std::make_shared<Texture> ();
        }
        sprite = makeSprite(texture, screenItem);
        break;
    }
    case TEXT:
    {
        // Get texture representing text
        texture = makeWordTexture(screenItem.name,
                                 screenItem.size,
                                 screenItem.color);
        texture->setSmooth(true);
        sprite = make_shared<Sprite>(*texture);
        break;
    }
    case BOX:
    {
        // Create a bordered box which will be used to hold text/image.
        RectangleShape shape( screenItem.size );
        // TODO: handle failure
        shape.setFillColor(screenItem.color);
        shape.setOutlineThickness(borderSize);
        shape.setOutlineColor(sf::Color(250, 200, 50));
        shape.setPosition(borderSize, borderSize);

        // Create texture we can draw to.
        m_rtexture.create(shape.getSize().x + 2*borderSize,
                          shape.getSize().y + 2*borderSize);
        m_rtexture.clear(sf::Color::Transparent);
        m_rtexture.draw(shape);
        m_rtexture.display();

        texture = make_shared<Texture>(m_rtexture.getTexture());
        texture->setSmooth(true);
        sprite = make_shared<Sprite>(*texture);
        break;
    }
    case HOTSPOT:
    {
        // Create texture we can draw to.
        m_rtexture.create(screenItem.size.x, screenItem.size.y);
        // TODO: handle failure

        // Create box for hotspot.
        RectangleShape shape(screenItem.size);
        // TODO: handle failure

        // Make it invisible
        shape.setFillColor(sf::Color::Transparent);

        // Make it into a texture
        m_rtexture.clear(sf::Color::Transparent);
        m_rtexture.draw(shape);
        m_rtexture.display();
        *texture = m_rtexture.getTexture();

        // Make a sprite out of it.
        sprite = makeSprite(texture, screenItem);

        break;
    }
    case BUTTON:
    {
        // Create texture we can draw to.
        m_rtexture.create(screenItem.size.x, screenItem.size.y);
        // TODO: handle failure

        // Create box to contain word.
        RectangleShape shape(sf::Vector2f(screenItem.size));
        // TODO: handle failure

        shape.setFillColor(screenItem.color);

        // Create word.
        Vec2 fontSize = {screenItem.size.y*.7f*.5f,0};
        texture = makeWordTexture(screenItem.name, fontSize, Color::Black);

        Sprite text(*texture);
        FloatRect size = text.getLocalBounds();
        float x = (shape.getSize().x - size.width) * .5;
        float y = (shape.getSize().y - size.height) * .5;
        text.move(x, y);

        // Put them together.
        m_rtexture.clear();
        m_rtexture.draw(shape);
        m_rtexture.draw(text);
        m_rtexture.display();
        *texture = m_rtexture.getTexture();

        // Make a sprite out of it.
        texture->setSmooth(true);

        sprite = std::make_shared<Sprite>();
        sprite->setTexture(*texture);

        break;
    }
    default:;
    }

    return sprite;
}


shared_ptr<Texture> Interface::makeWordTexture(const wstring& word,
                                               Vec2 size, Color color)
{
    // Determine how many newlines and chars/line are in string for sizing.
    size_t pos = 0;
    size_t lines = 0;
    size_t max_chars = 0;
    do{
        ++lines;
        size_t last_pos = pos;
        pos = word.find('\n', pos); // Find position of newline char.

        // Track the largest number of characters in a line.
        if( pos != string::npos)
        {// Then we have found one.
            if( pos - last_pos > max_chars ) // Then this line is longest yet.
                max_chars = pos - last_pos;
        }
        else
        {// We didn't find one, compare length of last line.
            if( word.size() - last_pos > max_chars )// Then this is the longest.
                max_chars = word.size() - last_pos;
        }
    }while( pos++ != string::npos );

    float char_width = size.x;
    float char_height = char_width * 2;
    int beginning = 0.5 * char_width;

    // Create a rendering context big enough to hold the text
    m_wordTexture.create(2*beginning+max_chars*char_width,
                         2*beginning+lines*char_height);
    m_wordTexture.clear(sf::Color::Transparent);

    int xPos = beginning; // Position of word relative to render context.
    int yPos = beginning; // Position of word relative to render context.

    // Draw each character to texture.
    m_fontshader.setParameter("color", color.r/255.0, color.g/255.0, color.b/255.0);
    for(auto ii = word.begin(); ii != word.end(); ii++)
    {
        wchar_t chr = *ii;
        if( chr != '\n' && chr != '\r')
        {
            if( !m_font.count(chr) ) // count() returns 0 if !exist
            {
                wcerr << "Error: Could not find char '" << chr
                     << "' when displaying text." << endl;
                loadCharacter(chr, "./images/error.png");
            }

            // Scale, position and draw character
            float x = char_width/m_font[chr].getTexture()->getSize().x;
            float y = char_height/m_font[chr].getTexture()->getSize().y;
            m_font[chr].setScale(x, y); // Size sprite to user desired size.
            m_font[chr].setPosition(xPos, yPos);
            xPos += char_width; // Adjust for next character.

            // Add current character to word
            m_wordTexture.draw(m_font[chr], &m_fontshader);
        }
        else
        {
            xPos = beginning;
            yPos += char_height;
        }
    }// End for

    m_wordTexture.display();
    //m_screenTextures.push_back(make_shared<Texture>(m_wordTexture.getTexture()));
    return make_shared<Texture>(m_wordTexture.getTexture());
    //m_textures.emplace(pair<wstring, shared_ptr<Texture>>
            //(word, make_shared<Texture>(m_wordTexture.getTexture())));
}

void Interface::draw(const Sprite& sprite, const ScreenItem& screenItem)
{
    if(screenItem.visible)
    {
        float r = screenItem.color.r/255.0;
        float g = screenItem.color.g/255.0;
        float b = screenItem.color.b/255.0;
        float a = screenItem.color.a/255.0;

        if(screenItem.engaged)
        {
            m_focusshader.setParameter("spriteWidth",
                                        sprite.getGlobalBounds().width);
            m_focusshader.setParameter("spriteHeight",
                                        sprite.getGlobalBounds().height);
            m_focusshader.setParameter("color", r, g, b, a);
            m_focusshader.setParameter("type", screenItem.type);
            m_focusshader.setParameter("selected", screenItem.selected);
            m_window->draw(sprite, &m_focusshader);
        }
        else
        {
            m_normalshader.setParameter("spriteWidth",
                                        sprite.getGlobalBounds().width);
            m_normalshader.setParameter("spriteHeight",
                                        sprite.getGlobalBounds().height);
            m_normalshader.setParameter("color", r, g, b, a);
            m_normalshader.setParameter("type", screenItem.type);
            m_normalshader.setParameter("selected", screenItem.selected);
            m_window->draw(sprite, &m_normalshader);
        }
    }
}

void Interface::render()
{
    m_window->clear();

    //this draws the background behind all other images
    if (m_screenSprites.size() > 0)
        draw(*(m_screenSprites[0]), m_screenItems[0]);

    // Draw the Screen sprites
    for(unsigned ii = 3; ii < m_screenSprites.size(); ++ii)
    {
        draw(*(m_screenSprites[ii]), m_screenItems[ii]);
    }

    // Draw hover text
    for(auto text : m_text)
    {
        m_window->draw(*text.second);
    }

    // Draw the HUD sprites
    for(unsigned ii = 0; ii < m_hudSprites.size(); ++ii)
    {
        draw(*(m_hudSprites[ii]), m_hudItems[ii]);
    }

    // Draw hover text for the hud
    for(auto text : m_hudtext)
    {
        m_window->draw(*text.second);
    }

    //this draws the instruction box for the current screen at the front of the other images
    if (m_screenSprites.size() > 2 && m_screenItems[1].visible)
    {
        draw(*(m_screenSprites[1]), m_screenItems[1]);
        draw(*(m_screenSprites[2]), m_screenItems[2]);
    }

    m_window->display();
}
