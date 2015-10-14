// Activity.h
// Joseph DeWilde
// 2/24/2014

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "screen.h"
#include "badge.h"
#include <memory>
using std::unique_ptr;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <QString>

class Activity : public Screen
{
public:
    Activity(string name, shared_ptr<Dictionary> dictionary = nullptr);
    ~Activity();

    void setChoices(size_t choices);
    void setRounds(size_t rounds);
    void setMaxScore(size_t score);
    void setBadgePiece(Badge, Piece);
    void setBadgePieceFromScene(Piece);
    void setDictionarySet(shared_ptr<Dictionary>);
    void setHelpMessage(wstring);
    void setID(string);
    void setRewardImage(wstring);
    void setRewardSoundFile(wstring);
    void setRewardSoundVolume(float);
    void setRewardSoundPitch(float);

    string getDictionaryName();

    size_t getChoices();
    size_t getRounds();
    size_t getMaxScore();
    wstring getRewardImage();
    wstring getRewardSoundFile();
    float getRewardSoundVolume();
    float getRewardSoundPitch();

    void addWordToDictionarySet(wstring, wstring, wstring, wstring);
    bool removeWordFromDictionarySet(wstring);

    void save();
    void load();

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(
           m_screenItems,
            m_BGM,
            m_dictionary,
            m_badge_piece,
            s_choices,
            s_rounds,
            s_maxScore,
            m_reward_image,
            m_reward_MusicItem
           );

        QString qStr = QString::fromStdWString(m_badge_piece.m_badge_name);
        cout << "m_badge_piece.m_badge_name: " << qStr.toStdString() << endl;
        qStr = QString::fromStdWString(m_badge_piece.m_image);
        cout << "m_badge_piece.m_image: " << qStr.toStdString() << endl;
    }

    Piece m_badge_piece;
private:
    string m_id;
    size_t m_choices;
    size_t m_rounds;
    size_t m_maxScore;
    string s_choices = "0";
    string s_rounds = "0";
    string s_maxScore = "0";
    wstring m_reward_image;
    MusicItem m_reward_MusicItem;
};

#endif // ACTIVITY_H

