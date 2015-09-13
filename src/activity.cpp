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

// Activity.cpp
// Joseph DeWilde
// 2/20/2014

#include "activity.h"
#include <string>
using std::string;
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/utility.hpp>
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;
#include <fstream>
using std::ifstream;
using std::ofstream;

Activity::Activity(string name, shared_ptr<Dictionary> dictionary, Piece piece)
: Screen(dictionary), m_id(name), m_choices(0), m_rounds(0), m_maxScore(0), m_badge_piece(piece)
{
}

Activity::~Activity()
{
	if (m_sound.getStatus() == sf::Sound::Playing)
		m_sound.stop();
	m_sound.~Sound();
	m_soundBuffer.~SoundBuffer();
}

void Activity::setChoices(size_t choices)
{
    m_choices = choices;
}

void Activity::setRounds(size_t rounds)
{
    m_rounds = rounds;
}

void Activity::setMaxScore(size_t score)
{
    m_maxScore = score;
}

void Activity::setRewardSoundFile(wstring newRewardSoundFileString)
{
	m_reward_MusicItem.file = newRewardSoundFileString;
}

void Activity::setRewardSoundVolume(float newVolume)
{
	m_reward_MusicItem.volume = newVolume;
}

void Activity::setRewardSoundPitch(float newPitch)
{
	m_reward_MusicItem.pitch = newPitch;
}

void Activity::setHelpMessage(wstring message)
{
    if (m_screenItems.size() > 2)
	{
		m_screenItems[2].size = { 10, 0 };
        std::swap(m_screenItems[2].name, message);
    }
}

void Activity::setBadgePiece(Piece piece)
{
    m_badge_piece = piece;
}

Piece Activity::getBadgePiece()
{
    return m_badge_piece;
}

wstring Activity::getRewardImage()
{
	return m_reward_image;
}

size_t Activity::getChoices()
{
    return m_choices;
}

size_t Activity::getRounds()
{
    return m_rounds;
}

size_t Activity::getMaxScore()
{
    return m_maxScore;
}

wstring Activity::getRewardSoundFile()
{
	return m_reward_MusicItem.file;
}

float Activity::getRewardSoundVolume()
{
	return m_reward_MusicItem.volume;
}

float Activity::getRewardSoundPitch()
{
	return m_reward_MusicItem.pitch;
}

void Activity::playRewardSound()
{  
	if (!m_soundBuffer.loadFromFile(sf::String(getRewardSoundFile())))
		cout << "loading sound failed: " << sf::String(getRewardSoundFile()).toAnsiString() << endl;

	cout << "loading sound time: " << m_soundBuffer.getDuration().asSeconds() << endl;

	m_sound.setBuffer(m_soundBuffer);
	m_sound.setVolume(getRewardSoundVolume());
	m_sound.setPitch(1);
	m_sound.setLoop(false);
	m_sound.play();
}

void Activity::save()
{
    std::ofstream out_stream(".//db//activities//" + m_id + ".act", std::ios::binary);

    if( !out_stream.fail() )
    {
        cereal::BinaryOutputArchive archive(out_stream);
        archive( *this );
    }
}

void Activity::load()
{
    std::ifstream in_stream(".//db//activities//" + m_id + ".act",std::ios::binary);

    if(in_stream && !in_stream.eof())
    {
        cereal::BinaryInputArchive archive(in_stream);
        archive(*this);
    }

	m_choices = stoi(s_choices);
	m_rounds = stoi(s_rounds);
	m_maxScore = stoi(s_maxScore);

	cout << "End Activity load" << endl;
}