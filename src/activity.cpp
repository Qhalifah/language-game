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

void Activity::setHelpMessage(wstring message)
{
    if (m_screenItems.size() > 2)
    {
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
}