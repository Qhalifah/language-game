#ifndef BGM_H
#define BGM_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
using cereal::BinaryInputArchive;
using cereal::BinaryOutputArchive;

class BGM{
    public:
    struct musicItem{
        string filename;
        int id;
        float volume;
        float pitch;

        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(filename,
               id,
               volume,
               pitch);
        }
    };
    BGM(){}
    bool loadFromFile(string filename)
    {
        ifstream ifile(filename, std::ios::binary);
        if(!ifile.is_open())
        {
            return false;
        }
        BinaryInputArchive archive(ifile);
        archive(*this);
        return true;
    }

    void saveToFile(string filename)
    {
        ofstream ofile(filename, std::ios::binary);
        BinaryOutputArchive archive(ofile);
        archive(*this);
    }

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(m_mus);
    }
    vector<musicItem> m_mus;
};
#endif // BGM_H
