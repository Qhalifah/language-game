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

// Badge class for badge objects
// Created by Charlie Carlson
// Scrum 1: Feb 18th 2014

// At the moment this is just a typedef for a string but
// It is contsructed so that in the future it may
// have additional data to reduce the number of required badge objects
// i.e. level, difficulty, quantity...

#ifndef __BADGE_H__
#define __BADGE_H__

#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
using std::cout;
using std::endl;

class Piece
{
public:
    std::wstring m_badge_name = L"none";

    std::wstring m_image = L"none";

    size_t m_id;

    bool operator < (const Piece& lhs) const
    {
        return m_id < lhs.m_id;
    }

    bool operator == (const Piece& lhs) const
    {
        return m_id == lhs.m_id;
    }

    template <class Archive>
	void serialize( Archive & ar )
	{
		ar( m_image, m_id, m_badge_name );
	}
};


class Badge
{
public:

    //stores the badges name
	std::wstring m_name = L"none";

    std::wstring m_image = L"none";

    std::vector<Piece> m_pieces;

    size_t m_total_pieces = 0;

    bool operator < (const Badge& lhs) const
    {
        return m_name < m_name;
    }

    bool operator == (const Badge& lhs) const
    {
        return m_name == m_name;
    }

    bool isComplete() const
    {
        return m_pieces.size() == m_total_pieces;
    }

	template <class Archive>
	void serialize( Archive & ar )
	{
		ar( m_name, m_image, m_pieces, m_total_pieces );
	}

	/*template <class Archive>
	void load( Archive & ar )
	{
		ar( m_name, m_image, m_pieces, m_total_pieces );
	}*/

};

#endif /* end define for __BADGE_H__ */
