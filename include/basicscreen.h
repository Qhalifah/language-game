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

#ifndef _BASICSCREEN_H
#define _BASICSCREEN_H

#include "screen.h"
#include <string>
using std::string;

class BasicScreen : public Screen
{
public:
	BasicScreen(string name = "BasicScreen", shared_ptr<Dictionary> dictionary = nullptr, string file_type = "txt");

    void save();
    void load();

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(m_screenItems, m_BGM, m_dictionary);
    }


private:
	string m_id;
	string m_file_location;
    string m_file_type; 
};

#endif // _BASICSCREEN_H
