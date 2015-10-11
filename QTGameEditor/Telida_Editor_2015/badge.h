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
using std::wstring;
#include <vector>
#include <unordered_map>
using std::unordered_map;

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

#include <QString>

class Piece
{
public:
     wstring m_badge_name = L"none";
     wstring m_image = L"none";
     size_t m_id = 1;
     std::string s_id = "none";

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
         s_id = std::to_string(m_id);

         ar( m_image,
             m_badge_name,
             s_id );

         m_id = stoi(s_id);
     }
};


class Badge
{
public:

    //stores the badges name
    wstring m_name = L"none";
    wstring m_image = L"none";
    std::unordered_map<size_t, Piece> m_pieces_map;
    size_t m_total_pieces = 0;
    unordered_map<size_t, wstring> m_piecesSave;

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
        return m_pieces_map.size() == m_total_pieces;
    }

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(m_name,
           m_image,
           m_pieces_map,
           m_total_pieces);
    }
};

#endif /* end define for __BADGE_H__ */
