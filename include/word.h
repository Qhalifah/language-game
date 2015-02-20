//Jeffrey Waldron
//2/11/2014
//Word.h

//This is the header for a Word Class
//A Word is a word in both ENG and DIN as well as locations to the DIN pronunciation and image

#ifndef WORD_H

#define WORD_H

#include <cstdlib>
using std::free;

#include <string>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>

class Word
{
    public:
    Word();
    //Words can only be made if they are given all three things words have
    Word(std::wstring const & dinName, std::wstring const & boyAudioLocation, std::wstring const & girlAudioLocation, std::wstring const & imageLocation);
    //Don't need this but hey, I might at one point
    ~Word();

    //Pretty basic get functions
    std::wstring getDinName();
    std::wstring getBoyAudioLocation();
    std::wstring getGirlAudioLocation();
    std::wstring getImageLocation();

    //There are no set functions as there should never be any need to set/change things words have

    template <class Archive>
    void save(Archive & ar) const
    {
        ar(
            m_dinName,
            m_boyAudioLocation,
            m_girlAudioLocation,
            m_imageLocation
           );
    }

    template <class Archive>
    void load(Archive & ar)
    {
        ar(
            m_dinName,
            m_boyAudioLocation,
            m_girlAudioLocation,
            m_imageLocation
           );
    }


    private:

    //Pretty basic member variables
    std::wstring m_dinName;
    std::wstring m_boyAudioLocation;
    std::wstring m_girlAudioLocation;
    std::wstring m_imageLocation;
};

#endif
