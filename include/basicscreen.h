
#ifndef _BASICSCREEN_H
#define _BASICSCREEN_H

#include "screen.h"
#include <string>
using std::string;

class BasicScreen : public Screen
{
public:
    BasicScreen(string name = "BasicScreen", shared_ptr<Dictionary> dictionary = nullptr, string location = ".//db//", string file_type = "txt");
    
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
