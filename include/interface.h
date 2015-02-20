// interface.h
// Shaun Bond
// 3/5/2014
//
//

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "screen.h"
#include "hud.h"
#include <SFML/Window.hpp>
using sf::RenderWindow;
using sf::View;
using sf::Event;
#include <SFML/Graphics.hpp>
using sf::RenderTexture;
using sf::Vector2f;
using sf::Sprite;
using sf::Texture;
using sf::Font;
using sf::Shader;
#include <SFML/Audio.hpp>
using sf::Music;
using sf::Sound;
using sf::SoundBuffer;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
using std::vector;
#include <string>
using std::wstring;
using std::string;
#include <map>
using std::unordered_map;

typedef unordered_map<wstring, shared_ptr<Texture>> TexMap;

class HUD;

class Interface
{
public:
    Interface();
    virtual ~Interface();

    void updateHud(size_t first, size_t last, const vector<ScreenItem>& screenItems);
    void updateHud(size_t index, const ScreenItem& data);
    void update(size_t first, size_t last, const vector<ScreenItem>& screenItems);
    void update(size_t index, const ScreenItem& data);
    void update(size_t index, const MusicItem& data);
    
    void changeMode(shared_ptr<Screen> new_screen);
    void changeHUD(shared_ptr<HUD> hud);
    void resize();
    
    void playBGM(size_t index);
    void stopBGM(size_t index);
    
    void playHudSound(size_t index);
    void playScreenSound(size_t index);

    const shared_ptr<sf::RenderWindow> getWindow() const;

    bool isCharSupported(wchar_t);

    int getScreenHit(float x, float y);
    int getHudHit(float x, float y);

private:
    //TexMap m_textures;
    Texture konstants;
    int borderSize = 5;
    float m_win_w = 800; // Initial value of window & used for resize
    float m_win_h = 600;
    float m_hud_width;
    float m_hud_height;
    float m_hud_scale;
    Shader m_focusshader;
    Shader m_normalshader;
    Shader m_fontshader;
    Vector2f m_scale;
    vector<shared_ptr<Texture>> m_font_textures;
    unordered_map<wchar_t, Sprite> m_font;
    shared_ptr<RenderWindow> m_window;
    View m_view;
    RenderTexture m_rtexture;
    RenderTexture m_wordTexture;

    unordered_map<wstring, shared_ptr<SoundBuffer>> m_soundBuffers;
    vector<ScreenItem> m_screenItems;
    vector<shared_ptr<Texture>> m_screenTextures;
    vector<shared_ptr<Sprite>> m_screenSprites;
    vector<shared_ptr<Sound>> m_screenSounds;

    vector<shared_ptr<Texture>> m_hudTextures;
    vector<shared_ptr<Sprite>> m_hudSprites;
    vector<ScreenItem> m_hudItems;
    vector<shared_ptr<Sound>> m_hudSounds;

    vector<shared_ptr<Music>> m_BGM;
    unordered_map<size_t, shared_ptr<Sprite>> m_text;
    unordered_map<size_t, shared_ptr<Texture>> m_textTextures;
    unordered_map<size_t, shared_ptr<Sprite>> m_hudtext;
    unordered_map<size_t, shared_ptr<Texture>> m_hudtextTextures;

    void render();

    void newHudObject(const ScreenItem&, size_t index);

    void newScreenObject(const ScreenItem&, shared_ptr<Sound>, size_t index);

    shared_ptr<Sprite> makeSprite(shared_ptr<Texture>&, const ScreenItem&);

    shared_ptr<Sprite> makeSpriteFromScreenItem(shared_ptr<Texture>&,
                                                const ScreenItem&);

    shared_ptr<Texture> makeWordTexture(const wstring&, Vec2 size, Color);

    void loadCharacter(wchar_t, const string& file);

    void positionSprite(shared_ptr<Sprite>, const ScreenItem&);

    void draw(const Sprite&, const ScreenItem&);

    void resizeSprite(shared_ptr<Sprite>, float scale, float h_adjust);

    void updateSprite(size_t index, const ScreenItem&);

    void updateHudSprite(size_t index, const ScreenItem&);
    
    void loadError(const wstring& file, const wstring& problem, const wstring& func);
};

#endif /* Interface_H_ */
