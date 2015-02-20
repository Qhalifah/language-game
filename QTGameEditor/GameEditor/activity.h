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

class Activity : public Screen
{
public:
    Activity(string name, shared_ptr<Dictionary> dictionary = nullptr);
    ~Activity();

    void setChoices(size_t choices);
    void setRounds(size_t rounds);
    void setMaxScore(size_t score);
    void setBadgePiece(Badge, Piece);
    void setHelpMessage(wstring);

    size_t getChoices();
    size_t getRounds();
    size_t getMaxScore();

    void save();
    void load();

    template<class Archive>
    void serialize(Archive & ar)
    {
        cout << "hi" << endl;
        ar(m_screenItems, m_BGM, m_dictionary, m_choices, m_rounds, m_maxScore);
        cout << "by" << endl;
    }

    Piece m_badge_piece;
    Badge m_badge;
private:
    string m_id;
    size_t m_choices;
    size_t m_rounds;
    size_t m_maxScore;
};

#endif // ACTIVITY_H
