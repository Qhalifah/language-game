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
	    cout << "piece1" << endl;
		ar( m_image, m_id, m_badge_name );
		cout << "piece2" << endl;
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
	    cout << "test1" << endl;
		ar( m_name, m_image, m_pieces, m_total_pieces );
		cout << "test2" << endl;
	}

	/*template <class Archive>
	void load( Archive & ar )
	{
		ar( m_name, m_image, m_pieces, m_total_pieces );
	}*/

};

#endif /* end define for __BADGE_H__ */
