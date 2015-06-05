
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

#include "basicscreen.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <fstream>
using std::ofstream;
using std::ifstream;


BasicScreen::BasicScreen(string name, shared_ptr<Dictionary> dictionary, string location, string file_type)
: Screen(dictionary), m_id(name), m_file_location(location), m_file_type(file_type)
{
    
}

void BasicScreen::save()
{
    std::ofstream os(m_file_location + m_id + "." + m_file_type, std::ios::binary | ofstream::out | std::ios::trunc);
    
    cereal::PortableBinaryOutputArchive archive(os);
    
    archive(m_screenItems, m_BGM, m_dictionary);
    //os.close();
}

void BasicScreen::load()
{
    std::ifstream is(m_file_location + m_id + "." + m_file_type, std::ios::binary | ifstream::in);
    
    if(!is.eof() && is)
    {
        cereal::PortableBinaryInputArchive archive(is);
        
        archive(m_screenItems, m_BGM, m_dictionary);
    }

	for (int ii = 0; ii < m_screenItems.size(); ++ii)
	{
		if (m_screenItems[ii].type == TEXT)
		{
			cout << "m_screenItems[ii].size.x: " << m_screenItems[ii].size.x << endl;
			cout << "m_screenItems[ii].size.y: " << m_screenItems[ii].size.y << endl;
		}
		else
		{
			cout << "m_screenItems[ii].size.y: " << m_screenItems[ii].size.y << endl;
		}

		m_screenItems[ii].size.x = 20;
		m_screenItems[ii].size.y = 20;
	}

	cout << "m_screenItems.size: " << endl;
}
