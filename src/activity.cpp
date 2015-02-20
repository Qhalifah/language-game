// Activity.cpp
// Joseph DeWilde
// 2/20/2014

#include "activity.h"
#include <string>
using std::string;
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
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

void Activity::save()
{
	std::ofstream os(".//db//activities//" + m_id + ".act", std::ios::binary);

    cereal::BinaryOutputArchive archive(os);

    archive( *this );
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
    //m_badge = badge;
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

void Activity::load()
{
    std::ifstream is(".//db//activities//" + m_id + ".act",std::ios::binary);

    if(!is.eof() && is)
    {
        cereal::BinaryInputArchive archive(is);

        archive(*this);
    }
}
