// ProfileWrangler for profilewrangler object
// Created by Charlie Carlson
// Scrum 1: Feb 18th 2014

// There will be one of these objects that will be owned by the "game"
// This class will be respnosible for loading all profiles from files
// it will also be responsible for saving profiles to file and removing profiles.

#ifndef __PROFILEWRANGLER_H__
#define __PROFILEWRANGLER_H__

//#include <cstdlib>
//using std::free;

#include "profile.h"

#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>

class ProfileWrangler
{
public:
    
    ProfileWrangler();

    // loads profile from file and returns current list of profiles
    // idea is that the game should always be in a state where you want to
    // load from file to get most updated list of profiles
    std::vector< std::shared_ptr<Profile> > loadProfiles(std::string fileName = "profilelist");
    
    std::vector< std::shared_ptr<Profile> > getProfiles();
    
    // adds profile, can add profile possible created before hand
    // or make a copy of a profile pased to it so it is handled
    // in the correct shared ptr format
    void addProfile(std::shared_ptr<Profile>);
    void addProfile(Profile);
    
    // removes profile. Must be handed shared ptr from vector
    // which can be gotten via loadprofiles
    // will call save profiles, hence removing the proflie for ever!
    void removeProfile(std::shared_ptr<Profile>);
    
    void removeProfile();
    
    // saves profiles to file
    void saveProfiles(std::string fileName = "profilelist") const;
    
    void setCurrentProfile(int index);
    
    std::shared_ptr<Profile> getCurrentProfile();
    
private:
    std::vector< std::shared_ptr<Profile> > m_profiles;
    
    int m_curr_profile_index;
};


#endif /* end define for __PROFILEWRANGLER_H__ */
