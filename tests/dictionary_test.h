//Jeffrey Waldron
//2/18/2014
//Test suite for Dictionary class

//Dictionary_test.h

#include "dictionary.h"
#include "word.h"
#include "catch.hpp"
#include <string>


SCENARIO("Word Class", "[Word_class]")
{
    GIVEN("A newly constructed Word Object")
    {
        Word newWord;

        WHEN("The getters are called")
        {
            THEN("It should be 'EMPTY' across the board")
            {
                REQUIRE(newWord.getDinName() == L"EMPTY");
                REQUIRE(newWord.getBoyAudioLocation() == L"EMPTY");
                REQUIRE(newWord.getGirlAudioLocation() == L"EMPTY");
                REQUIRE(newWord.getImageLocation() == L"EMPTY");
            }
        }
    }

    GIVEN("A Word with Name1, Location1B, Location1G, LocationA")
    {
        Word newWord(L"Name1", L"Location1B", L"Location1G", L"LocationA");

        WHEN("The getters are called")
        {
            THEN("Should return the correct strings")
            {
                REQUIRE(newWord.getDinName() == L"Name1");
                REQUIRE(newWord.getBoyAudioLocation() == L"Location1B");
                REQUIRE(newWord.getGirlAudioLocation() == L"Location1G");
                REQUIRE(newWord.getImageLocation() == L"LocationA");
            }
        }
    }
}

SCENARIO("Dictionary Class", "[Dictionary_Class")
{
    GIVEN("A newly constructed Dictionary and Word(Name1, Location1B, Location1G, LocationA")
    {
        Dictionary newDic;
        Word newWord(L"Name1", L"Location1B", L"Location1G", L"LocationA");

        WHEN("New Word is added")
        {
            newDic.addWord(newWord);

            THEN("Getting New Word should match up")
            {
                REQUIRE(newDic[L"Name1"].getDinName() == L"Name1");
                REQUIRE(newDic[L"Name1"].getBoyAudioLocation() == L"Location1B");
                REQUIRE(newDic[L"Name1"].getGirlAudioLocation() == L"Location1G");
                REQUIRE(newDic[L"Name1"].getImageLocation() == L"LocationA");
            }
        }
    }
    GIVEN("A newly constructed Dictionary and Word(Name1, Location1, LocationA). It will be Saved, something added, then loaded and the added item shouldn't appear")
    {
        Dictionary newDic;
        Word newWord(L"Name1", L"Location1B", L"Location1G", L"LocationA");

        WHEN("New Word is added")
        {
            newDic.addWord(newWord);

            THEN("Getting New Word should match up")
            {
                REQUIRE(newDic[L"Name1"].getDinName() == L"Name1");
                REQUIRE(newDic[L"Name1"].getBoyAudioLocation() == L"Location1B");
                REQUIRE(newDic[L"Name1"].getGirlAudioLocation() == L"Location1G");
                REQUIRE(newDic[L"Name1"].getImageLocation() == L"LocationA");
            }
        }
        WHEN("Dictionary is saved and then a new word added")
        {
            Word newWord(L"Name1", L"Location1B", L"Location1G", L"LocationA");
            newDic.addWord(newWord);
            newDic.saveDictionary("test.xml");
            Word newerWord(L"Name11", L"Location11B", L"Location11G", L"LocationAA");
            newDic.addWord(newerWord);
            THEN("Check if newer word was added")
            {
                    REQUIRE(newDic[L"Name11"].getDinName() == L"Name11");
                    REQUIRE(newDic[L"Name11"].getBoyAudioLocation() == L"Location11B");
                    REQUIRE(newDic[L"Name11"].getGirlAudioLocation() == L"Location11G");
                    REQUIRE(newDic[L"Name11"].getImageLocation() == L"LocationAA");
            }
            newDic.loadDictionary("test.xml");
            THEN("Loading word and seeing if it matches")
            {
                REQUIRE(newDic[L"Name1"].getDinName() == L"Name1");
                REQUIRE(newDic[L"Name1"].getBoyAudioLocation() == L"Location1B");
                REQUIRE(newDic[L"Name1"].getGirlAudioLocation() == L"Location1G");
                REQUIRE(newDic[L"Name1"].getImageLocation() == L"LocationA");
                REQUIRE(newDic[L"Name11"].getDinName() == L"EMPTY");
                REQUIRE(newDic[L"Name11"].getBoyAudioLocation() == L"EMPTY");
                REQUIRE(newDic[L"Name11"].getGirlAudioLocation() == L"EMPTY");
                REQUIRE(newDic[L"Name11"].getImageLocation() == L"EMPTY");
            }
        }
    }
}
