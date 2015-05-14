//
/////*
//// * makefiles.cpp
//// *
//// *  Created on: Apr 4, 2014
//// *      Author: shaun
//// */
//
//
//
//#include "compile.h"
//
//#include "interface.h"
//#include "basicscreen.h"
//#include "profilecontroller.h"
//#include "map.h"
//#include "screen.h"
//using mine::Color;
//#include "scene.h"
//#include "activity.h"
//#include <vector>
//#include <string>
//using std::wstring;
//using std::stoul;
//#include <fstream>
//using std::wifstream;
//using std::wofstream;
//using std::ifstream;
//#include <iostream>
//using std::cerr;
//using std::wcerr;
//using std::cout;
//using std::endl;
//
//
//typedef wifstream flexifstream;
//typedef wstring   flexstring;
//
//flexstring string_to_flexstring(string str);
//string flex_to_string(flexstring flexstr);
//
//void error(const flexstring& input);
//ScreenItemType getType(flexifstream& fin);
//Vec2 getVec2(flexifstream& fin);
//bool getBool(flexifstream& fin);
//Color getColor(flexifstream& fin);
//shared_ptr<ScreenItem> getScreenItem(flexifstream& fin);
//shared_ptr<MusicItem> getMusicItem(flexifstream& fin);
//shared_ptr<Dictionary> getDictionary(flexifstream& fin);
//void writeProfilePageFile(flexifstream& fin);
//void writeMapFile(flexifstream& fin);
//void writeSceneFile(flexifstream& fin);
//void writeActivityFile(flexifstream& fin);
//
//
//#if defined (_MSC_VER)
//    #include <codecvt>
//	#include <io.h>
//	#include <fcntl.h>
//#endif
//
//int main()
//{
//    //setlocale(LC_ALL, "en_US.UTF-8");
//    //std::locale loc (std::locale(), new std::codecvt_utf8<char32_t>);
//    //std::locale::global(loc);
//
//    flexifstream fin("db/content.txt");
//	cout << "hi" << endl;
//    #if defined(__clang__)
//        fin.imbue(std::locale("en_US.UTF-8"));
//
//    #elif defined(__GNUC__) || defined(__GNUG__)
//        fin.imbue(std::locale("en_US.UTF-8"));
//		
//   #elif defined(_MSC_VER)
//	  _setmode(_fileno(stdout),_O_WTEXT);
//      fin.imbue(std::locale(fin.getloc(),new std::codecvt_utf8<wchar_t, 0xffff, std::consume_header>));
//    //fin.imbue(std::locale("en_US.UTF-8"));
//	  cout << "hi2" << endl;
//    #endif
//
//	   //std::wcout << "abcdefghijklmnopqrstuvwxyzł.'ˋ" << std::endl;
//	   /*while (fin)
//	   {
//		   std::wstring t_string;
//		  std::getline(fin,t_string);
//		   std::wcout << t_string << std::endl;
//	   }
//
//	   return -2;*/
//
//    if( !fin )
//    {
//        cerr << "Error opening content.txt" << endl;
//        exit(0);
//    }
//
//    flexstring input;
//
//    // Skip documentation, assume MAP is first declaration
//    do {
//        fin >> input;
//    }while( (input != L"MAP") && !fin.eof() );
//
//    // Look for file type definitions and write out files.
//    while( !fin.eof() )
//    {
//        if( input == L"PROFILE_PAGE")
//        {
//            writeProfilePageFile(fin);
//        }
//        else if( input == L"MAP" )
//        {
//            writeMapFile(fin);
//        }
//        else if( input == L"SCENE" )
//        {
//            writeSceneFile(fin);
//        }
//        else if( input == L"ACTIVITY" )
//        {
//            writeActivityFile(fin);
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    fin.close();
//
////    cout << "Done" << endl;
//    return 0;
//}
//
//flexstring string_to_flexstring(string str)
//{
//    return flexstring(str.begin(), str.end());
//}
//
//string flex_to_string(flexstring flexstr)
//{
//    return string(flexstr.begin(), flexstr.end());
//}
//
//void error(const flexstring& input)
//{
//    wcerr << L"Error: " << input << endl;
//    exit(0);
//}
//
//ScreenItemType getType(flexifstream& fin)
//{
//    ScreenItemType type;
//    flexstring input;
//    fin >> input;
//
//    if( input == L"IMAGE" )
//    {
//        type = IMAGE;
//    }
//    else if( input == L"TEXT" )
//    {
//        type = TEXT;
//    }
//    else if( input == L"BOX")
//    {
//        type = BOX;
//    }
//    else if( input == L"BUTTON" )
//    {
//        type = BUTTON;
//    }
//    else if( input == L"HOTSPOT" )
//    {
//        type = HOTSPOT;
//    }
//    else
//    {
//        error(input);
//    }
//
//    return type;
//}
//
//GameType getGameType(flexstring& input)
//{
//    GameType type;
//
//    if( input == L"MATCHING" )
//    {
//        type = MATCHING;
//    }
//    else if( input == L"PAIR" )
//    {
//        type = PAIR;
//    }
//    else
//    {
//        error(input);
//    }
//
//    return type;
//}
//
//Vec2 getVec2(flexifstream& fin)
//{
//    Vec2 retVal;
//
//    fin >> retVal.x;
//    fin >> retVal.y;
//
//    return retVal;
//}
//
//bool getBool(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input;
//
//    if( input == L"false" )
//        return false;
//    else if( input == L"true" )
//        return  true;
//    else
//        error(input);
//
//    return false; // Dummy return to make compiler happy
//}
//
//Color getColor(flexifstream& fin)
//{
//    int red;
//    int green;
//    int blue;
//    int alpha;
//
//    fin >> red;
//    fin >> green;
//    fin >> blue;
//    fin >> alpha;
//
//    Color retVal(red, green, blue, alpha);
//    return retVal;
//}
//
//shared_ptr<ScreenItem> getScreenItem(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input;
//
//    shared_ptr<ScreenItem> newScreenItem(new ScreenItem);
//    while( (input != L"END") && !fin.eof() )
//    {
//        if( input == L"TYPE" )
//        {
//            newScreenItem->type = getType(fin);
//        }
//        else if( input == L"NAME" )
//        {
//            fin >> newScreenItem->name;
//        }
//        else if( input == L"SOUND" )
//        {
//            fin >> newScreenItem->sound;
//        }
//        else if( input == L"HOVER_TEXT" )
//        {
//            fin >> newScreenItem->hover_text;
//        }
//        else if( input == L"POSITION" )
//        {
//            newScreenItem->position = getVec2(fin);
//        }
//        else if( input == L"SIZE" )
//        {
//            newScreenItem->size = getVec2(fin);
//        }
//        else if( input == L"VOLUME" )
//        {
//            fin >> newScreenItem->volume;
//        }
//        else if( input == L"PITCH" )
//        {
//            fin >> newScreenItem->pitch;
//        }
//        else if( input == L"VISIBLE" )
//        {
//            newScreenItem->visible = getBool(fin);
//        }
//        else if( input == L"COLOR" )
//        {
//            newScreenItem->color = getColor(fin);
//        }
//        else if( input == L"BEHAVIOR" )
//        {
//            int t_int;
//            fin >> t_int;
//            newScreenItem->behavior = t_int;
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    return newScreenItem;
//}
//
//shared_ptr<MusicItem> getMusicItem(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input;
//
//    shared_ptr<MusicItem> newMusicItem(new MusicItem);
//    while( (input != L"END") && !fin.eof() )
//    {
//        if( input == L"FILE" )
//        {
//            fin >> newMusicItem->file;
//        }
//        else if( input == L"VOLUME" )
//        {
//            fin >> newMusicItem->volume;
//        }
//        else if( input == L"PITCH" )
//        {
//            fin >> newMusicItem->pitch;
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    return newMusicItem;
//}
//
//// Reads in words and creates a dictionary
//shared_ptr<Dictionary> getDictionary(flexifstream& fin)
//{
//    shared_ptr<Dictionary> dict( new Dictionary() );
//    flexstring input;
//    fin >> input;
//
//    flexstring word;
//    flexstring male;
//    flexstring female;
//    flexstring image;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        fin >> word;
//        fin >> input; // Throw away identifier for next item
//
//        fin >> male;
//        fin >> input;
//
//        fin >> female;
//        fin >> input;
//
//        fin >> image;
//        fin >> input;
//
//        dict->addWord(*new Word(word, male, female, image));
//    }
//
//    return dict;
//}
//
//// Reads in a list of activities for a scene and returns the list
//vector<MiniGame> getActivities(flexifstream& fin)
//{
//    vector<MiniGame> activities;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        MiniGame game;
//
//        game.first = getGameType(input);
//        fin >> input;
//
//        game.second = flex_to_string(input);
//        fin >> input;
//
//        activities.push_back(game);
//    }
//
//    return activities;
//}
//
//// Reads in a list of activity requirements and returns it as a map
//map<set<unsigned>, unsigned> getActivityReqs(flexifstream& fin)
//{
//    map<set<unsigned>, unsigned> reqs;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        set<unsigned> activities;
//        while( (input != L":") && !fin.eof() )
//        {
//            unsigned number = stoul(input);
//            activities.insert(number);
//            fin >> input;
//        }
//
//        unsigned number;
//        fin >> number;
//        reqs.insert(make_pair(activities, number));
//
//        fin >> input;
//    }
//
//    return reqs;
//}
//
//Badge getBadge(flexifstream& fin)
//{
//    Badge badge;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        if( input == L"NAME" )
//        {
//            fin >> badge.m_name;
//        }
//        else if( input == L"IMAGE" )
//        {
//            fin >> badge.m_image;
//        }
//        else if( input == L"PIECES" )
//        {
//            fin >> badge.m_total_pieces;
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    return badge;
//}
//
//Piece getBadgePiece(flexifstream& fin)
//{
//    Piece badge;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        if( input == L"ID" )
//        {
//            fin >> badge.m_id;
//        }
//        else if( input == L"IMAGE" )
//        {
//            fin >> badge.m_image;
//        }
//        else if( input == L"BADGE_NAME")
//        {
//            fin >> badge.m_badge_name;
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    return badge;
//}
//
//// Reads in a list of scenes returns it as a map
//unordered_map<unsigned, string> getScenes(flexifstream& fin)
//{
//    unordered_map<unsigned, string> scenes;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        unsigned number = stoul(input);
//        fin >> input; // throw away :
//        fin >> input; // name of the scene
//        string name = flex_to_string(input);
//        scenes.insert(make_pair(number, name));
//
//        fin >> input;
//    }
//
//    return scenes;
//}
//
//// Reads in a list of scene requirements and returns it as a map
//unordered_map<unsigned, set<std::wstring>> getSceneReqs(flexifstream& fin)
//{
//    unordered_map<unsigned, set<std::wstring>> reqs;
//    flexstring input;
//    fin >> input;
//
//    while( (input != L"END") && !fin.eof() )
//    {
//        unsigned number = stoul(input);
//        fin >> input; // throw away :
//        fin >> input; // first badge
//
//        set<std::wstring> badges;
//        while( (input != L":") && !fin.eof() )
//        {
//            std::wstring badge = input;
//            badges.insert(badge);
//
//            fin >> input;
//        }
//
//        reqs.insert(make_pair(number, badges));
//
//        fin >> input;
//    }
//
//    return reqs;
//}
//
//// Reads in information and writes out map file which can be read by game.
//void writeProfilePageFile(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input; // First input after MAP should be the name
//
//    //Hack to convert to string without code that'll make your mother cry.
//    string name = flex_to_string(input);
//
//    /*wofstream out("name.txt");
//    out << input;
//    out.close();
//
//    ifstream in("name.txt");
//    in >> name;
//    in.close();*/
//
//    // Make a map object, and empty vectors for ScreenItems and MusicItems
//    std::shared_ptr<BasicScreen>    ppage(new BasicScreen(name ,nullptr, ".//db//profilepage//", "pp"));
//    std::vector<ScreenItem> images;
//    std::vector<MusicItem>  bgms;
//
//    fin >> input;
//
//    // Look for ScreenItems, MusicItems, or Dictionaries and add to object.
//    while( (input != L"END_PROFILE_PAGE") && !fin.eof() )
//    {
//        if( input == L"SCREEN_ITEM" )
//        {
//            images.push_back(*getScreenItem(fin));
//        }
//        else if( input == L"MUSIC_ITEM" )
//        {
//            bgms.push_back(*getMusicItem(fin));
//        }
//        else if( input == L"DICTIONARY" )
//        {
//            ppage->setDictionary(getDictionary(fin));
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    ppage->setScreenItems(images);
//    ppage->setBGM(bgms);
//
//    ppage->save();
//}
//
//// Reads in information and writes out map file which can be read by game.
//void writeMapFile(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input; // First input after MAP should be the name
//
//    //Hack to convert to string without code that'll make your mother cry.
//    string name = flex_to_string(input);
//
//    /*wofstream out("name.txt");
//    out << input;
//    out.close();
//
//    ifstream in("name.txt");
//    in >> name;
//    in.close();*/
//
//    // Make a map object, and empty vectors for ScreenItems and MusicItems
//    std::shared_ptr<Map>    map(new Map(name));
//    std::vector<ScreenItem> images;
//    std::vector<MusicItem>  bgms;
//
//    fin >> input;
//
//    // Look for ScreenItems, MusicItems, or Dictionaries and add to object.
//    while( (input != L"END_MAP") && !fin.eof() )
//    {
//        if( input == L"SCREEN_ITEM" )
//        {
//            images.push_back(*getScreenItem(fin));
//        }
//        else if( input == L"MUSIC_ITEM" )
//        {
//            bgms.push_back(*getMusicItem(fin));
//        }
//        else if( input == L"DICTIONARY" )
//        {
//            map->setDictionary(getDictionary(fin));
//        }
//        else if( input == L"SCENES" )
//        {
//            unordered_map<unsigned, string> scenes = getScenes(fin);
//            map->setScenes(scenes);
//        }
//        else if( input == L"REQUIREMENTS" )
//        {
//            unordered_map<unsigned, set<std::wstring>> reqs = getSceneReqs(fin);
//            map->setScenePrereqs(reqs);
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    map->setScreenItems(images);
//    map->setBGM(bgms);
//
//    map->save();
//}
//
//// Reads in information and writes out scene file which can be read by game.
//void writeSceneFile(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input; // First input after SCENE should be the name
//
//    string name = flex_to_string(input);
//
//    /*wofstream out("name.txt");
//    out << input;
//    out.close();
//
//    ifstream in("name.txt");
//    in >> name;
//    in.close();*/
//
//    // Make a map object, and empty vectors for ScreenItems and MusicItems
//    std::shared_ptr<Scene>  scene(new Scene(name));
//    std::vector<ScreenItem> images;
//    std::vector<MusicItem>  bgms;
//
//    fin >> input;
//
//    // Look for ScreenItems, MusicItems, or Dictionaries and add to object.
//    while( (input != L"END_SCENE") && !fin.eof() )
//    {
//        if( input == L"SCREEN_ITEM" )
//        {
//            images.push_back(*getScreenItem(fin));
//        }
//        else if( input == L"MUSIC_ITEM" )
//        {
//            bgms.push_back(*getMusicItem(fin));
//        }
//        else if( input == L"DICTIONARY" )
//        {
//            scene->setDictionary(getDictionary(fin));
//        }
//        else if( input == L"ACTIVITIES" )
//        {
//            vector<MiniGame> activities = getActivities(fin);
//            scene->setActivities(activities);
//        }
//        else if( input == L"REQUIREMENTS" )
//        {
//            map<set<unsigned>, unsigned> reqs = getActivityReqs(fin);
//            scene->setRequirements(reqs);
//        }
//        else if( input == L"BADGE" )
//        {
//            Badge badge = getBadge(fin);
//            scene->setBadge(badge);
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    scene->setScreenItems(images);
//    scene->setBGM(bgms);
//
//    scene->save();
//}
//
//// Reads in information and writes out activity file which can be read by game.
//void writeActivityFile(flexifstream& fin)
//{
//    flexstring input;
//    fin >> input; // First input after ACTIVITY should be the name
//
//    string name = flex_to_string(input);
//
//    /*wofstream out("name.txt");
//    out << input;
//    out.close();
//
//    ifstream in("name.txt");
//    in >> name;
//    in.close();*/
//
//    // Make a map object, and empty vectors for ScreenItems and MusicItems
//    std::shared_ptr<Activity> activity(new Activity(name));
//    std::vector<ScreenItem>   images;
//    std::vector<MusicItem>    bgms;
//
//    fin >> input;
//
//    size_t t_bucket = 0;
//    // Look for ScreenItems, MusicItems, or Dictionaries and add to object.
//    // Also set choices, rounds and maxScore.
//    while( (input != L"END_ACTIVITY") && !fin.eof() )
//    {
//        if( input == L"SCREEN_ITEM" )
//        {
//            images.push_back(*getScreenItem(fin));
//        }
//        else if( input == L"MUSIC_ITEM" )
//        {
//            bgms.push_back(*getMusicItem(fin));
//        }
//        else if( input == L"DICTIONARY" )
//        {
//            activity->setDictionary(getDictionary(fin));
//        }
//        else if( input == L"CHOICES")
//        {
//            fin >> t_bucket;
//            activity->setChoices(t_bucket);
//        }
//        else if( input == L"ROUNDS")
//        {
//            fin >> t_bucket;
//            activity->setRounds(t_bucket);
//        }
//        else if( input == L"MAX_SCORE")
//        {
//            fin >> t_bucket;
//            activity->setMaxScore(t_bucket);
//        }
//        else if( input == L"BADGE_PIECE")
//        {
//            Badge badge;
//            fin >> badge.m_name;
//            Piece piece = getBadgePiece(fin);
//            activity->setBadgePiece(piece);
//        }
//        else
//            error(input);
//
//        fin >> input;
//    }
//
//    activity->setScreenItems(images);
//    activity->setBGM(bgms);
//
//    activity->save();
//}
//
//
//
