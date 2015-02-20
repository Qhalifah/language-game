

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
    std::ofstream os(m_file_location + m_id + "." + m_file_type, std::ios::binary | std::ios_base::binary | ofstream::out | std::ios::trunc);
    
    cereal::PortableBinaryOutputArchive archive(os);
    
    archive(m_screenItems, m_BGM, m_dictionary);
    //os.close();
}

void BasicScreen::load()
{
    std::ifstream is(m_file_location + m_id + "." + m_file_type, std::ios::binary | std::ios_base::binary | ifstream::in);
    
    if(!is.eof() && is)
    {
        cereal::PortableBinaryInputArchive archive(is);
        
        archive(m_screenItems, m_BGM, m_dictionary);
    }
}