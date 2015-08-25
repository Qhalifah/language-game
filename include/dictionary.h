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

//Jeffrey Waldron
//2/11/2014
//Dictionary.h

//This is the header file for the Dictionary class
//A Dictionary is just a bunch of words (see word.h)

#ifndef DICTIONARY_H

#define DICTIONARY_H

#include <cstdlib>
using std::free;

#include <string>
#include "word.h"
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
#include <map>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>

class Dictionary
{
    public:

    //When passed nothing, a blank Dictionary will be created
    Dictionary();
    //when passed a location, a Dictionary will be made from that XML file
    //By default it grabs all the words which are located here - TODO
    Dictionary(std::string const & fileLocation);
    //Adds a word to the dictionary
    //In case we ever need it
    ~Dictionary();
    //Adds a word to the Dictionary
    //NOTE: If the word given already exists, this overwrites that value
    //NOTE: Can't be constant as it's a Map :(
    void addWord(Word & given);
    //Loads from file
    void loadDictionary(std::string const & fileLocaion);
    //Saves to a file
    void saveDictionary(std::string const & fileLocation);
    //Get Word when passed a string
    Word operator [](std::wstring given);
    //Get a random word
    Word getRandomWord();
    //Gets size of dictionary
    unsigned int getSize();
    //Add a word with input
    void addInputWord();
    //Used for debugging. Prints all words
    void printWords();


    template <class Archive>
    void save(Archive & ar) const
    {
		ar(
			dictSet_id,
            m_words
           );
    }

    template <class Archive>
    void load(Archive & ar)
    {
        cout << "dict" << endl;
		ar(
			dictSet_id,
            m_words
           );
           cout << "dict2" << endl;
    }

    private:

    std::map<std::wstring, Word> m_words;
	std::string dictSet_id = "None";
};

#endif // DICTIONARY_H
