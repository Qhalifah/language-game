#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
using std::getline;

class Communication{
public:
    enum action {ADD, REMOVE, VOLUME, PITCH, NONE};;
    Communication():m_stop(false), m_action(NONE),
    m_vol(50), m_pitch(50), m_filename(""){}
    bool loadFromFile(string filename)
    {
        ifstream ifile(filename);
        if(!ifile.is_open())
        {
            return false;
        }
        string line = "";
        int num;
        ifile >> m_stop;
        getline(ifile, line);
        ifile >> num;
        m_action = static_cast<action>(num);
        getline(ifile, line);
        ifile >> m_vol;
        getline(ifile, line);
        ifile >> m_pitch;
        getline(ifile, line);
        ifile >> m_id;
        getline(ifile, line);
        m_filename = "";
        getline(ifile, m_filename);
        ifile.close();
        return true;
    }

    bool saveToFile(string filename)
    {
        ofstream ofile(filename);
        if(!ofile.is_open())
            return false;
        ofile << m_stop;
        ofile << "\n";
        ofile << m_action;
        ofile << "\n";
        ofile << m_vol;
        ofile << "\n";
        ofile << m_pitch;
        ofile << "\n";
        ofile << m_id;
        ofile << "\n";
        ofile << m_filename;
        ofile.close();
        m_action = NONE;
        return true;
    }

    bool m_stop;
    action m_action;
    float m_vol;
    float m_pitch;
    int m_id;
    string m_filename;
};

#endif // COMMUNICATION_H
