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

Activity::Activity(string name, shared_ptr<Dictionary> dictionary)
: Screen(dictionary), m_id(name), m_choices(0), m_rounds(0), m_maxScore(0)
{
    Badge *b = new Badge();
    Piece *p = new Piece();
    setBadgePiece(*b, *p);
}

Activity::~Activity()
{
}

void Activity::setChoices(size_t choices)
{
    m_choices = choices;
    s_choices = std::to_string(choices);
}

void Activity::setRounds(size_t rounds)
{
    m_rounds = rounds;
    s_rounds = std::to_string(rounds);
}

void Activity::setMaxScore(size_t score)
{
    m_maxScore = score;
    s_maxScore = std::to_string(score);
}

void Activity::setHelpMessage(wstring message)
{
    if (m_screenItems.size() > 2)
    {
        std::swap(m_screenItems[2].name, message);
    }
}

void Activity::setBadgePiece(Badge badge, Piece piece)
{
    m_badge_piece = piece;
}

void Activity::setBadgePieceFromScene(Piece p)
{
    load();
    m_badge_piece = p;
    save();
}

void Activity::setDictionarySet(shared_ptr<Dictionary> newDictionary)
{
    std::swap(m_dictionary, newDictionary);
}

string Activity::getDictionaryName()
{
    return m_dictionary->getDictSetID();
}

void Activity::setRewardImage(wstring newRewardImageFileString)
{
    m_reward_image = newRewardImageFileString;
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

wstring Activity::getRewardImage()
{
    return m_reward_image;
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

void Activity::addWordToDictionarySet(wstring name, wstring femVoice, wstring maleVoice, wstring imgLoc)
{
    m_dictionary->addWord(Word(name, femVoice, maleVoice, imgLoc));
}

bool Activity::removeWordFromDictionarySet(wstring wordName)
{
    return m_dictionary->removeWord(wordName);
}

void Activity::save()
{
    if(m_screenItems[0].name == L"")
    {
        cout << "No background image, putting None in name so no crash occurrs." << endl;
        m_screenItems[0].name = L"None";
    }

	s_choices = std::to_string(m_choices);
	s_rounds = std::to_string(m_rounds);
	s_maxScore = std::to_string(m_maxScore);

    cout << "save activity to " + m_id + ".act" << endl;
    QString str = QString::fromStdWString(m_badge_piece.m_badge_name);
    cout << "m_badge_name: " << str.toStdString() << endl;
    str = QString::fromStdWString(m_badge_piece.m_image);
    cout << "m_image: " << str.toStdString() << endl;
    cout << "m_id: " << m_badge_piece.m_id << endl;

    std::ofstream os(".//db//activities//" + m_id + ".act", std::ios::binary);

    cereal::BinaryOutputArchive archive(os);

    archive( *this );

    cout << "Help Box is (" << m_screenItems[1].size.x << ", " << m_screenItems[1].size.y << ") at (" << m_screenItems[1].position.x << ", " << m_screenItems[1].position.y << ")" << endl;
}

void Activity::load()
{
    cout << "activity load started." << endl;

    std::ifstream is(".//db//activities//" + m_id + ".act",std::ios::binary);

    if(!is.eof() && is)
    {
        cereal::BinaryInputArchive archive(is);
        archive(*this);
    }

    m_choices = stoi(s_choices);
    m_rounds = stoi(s_rounds);
    m_maxScore = stoi(s_maxScore);

    cout << "Inside Activity::load() " << endl;
    QString str = QString::fromStdWString(m_badge_piece.m_badge_name);
    cout << "m_badge_name: " << str.toStdString() << endl;
    str = QString::fromStdWString(m_badge_piece.m_image);
    cout << "m_image: " << str.toStdString() << endl;
    cout << "m_id: " << m_badge_piece.m_id << endl;
    cout << "Choices: " << m_choices << endl;
    cout << "Rounds: " << m_rounds << endl;
    cout << "MaxScore: " << m_maxScore << endl;
    cout << "Help Box is (" << m_screenItems[1].size.x << ", " << m_screenItems[1].size.y << ") at (" << m_screenItems[1].position.x << ", " << m_screenItems[1].position.y << ")" << endl;

    cout << "activity load finished." << endl;    
}
