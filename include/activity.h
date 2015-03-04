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
