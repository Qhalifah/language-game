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
    Activity(string name, shared_ptr<Dictionary> dictionary = nullptr, Piece piece = Piece());
    ~Activity();

    void setChoices(size_t choices);
    void setRounds(size_t rounds);
    void setMaxScore(size_t score);
    
    void setBadgePiece(Piece);
    Piece getBadgePiece(); 
    
    void setHelpMessage(wstring);

    size_t getChoices();
    size_t getRounds();
    size_t getMaxScore();

    void save();
    void load();
    
    

    template<class Archive>
    void serialize(Archive & ar)
    {
        //cout << "hi" << endl;
        ar(m_screenItems, m_BGM, m_dictionary, m_choices, m_rounds, m_maxScore, m_badge_piece);
        //cout << "by" << endl;
    }

    
    //Badge m_badge;
private:
    string m_id;
    size_t m_choices;
    size_t m_rounds;
    size_t m_maxScore;
    
    Piece m_badge_piece;
};

#endif // ACTIVITY_H
