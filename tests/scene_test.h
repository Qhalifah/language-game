// Scene_test.h
// Joseph DeWilde
// 2/19/2014
//
// Unit test file for the Screen class

#include "scene.h"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Audio/Music.hpp"
#include <string>
using std::string;
/*
SCENARIO("Scene Class", "[Scene_class]")
{
    GIVEN("A Scene cereal file and resources to construct a Scene")
    {
        string BGMfileName = "audio/background/nature.ogg";
        string BGIfileName = "images/Scenes/river.jpg";
        string reqFileName = "db/badges/fishingLicense.cereal";
        string rewardFileName = "db/badges/fish.cereal";
        string act1 = "db/activities/fishingWithRod.cereal";
        string act2 = "db/activities/fishingWithNet.cereal";
        string obj1 = "db/objects/boat.cereal";
        string obj2 = "db/objects/net.cereal";
        string obj3 = "db/objects/rod.cereal";

        float volume = 50;

        float pitch = 1.2;
        Scene scene1;
        vector<string> BGM = {
            BGMfileName
        };
        scene1.setBGM(BGM);
        vector<float> volumes = {
            volume
        };
        scene1.setBGMVolume(volumes);
        vector<float> pitches = {
            pitch
        };
        scene1.setBGMPitch(pitches);
        scene1.setBGI(BGIfileName);
        scene1.setRequirement(reqFileName);
        scene1.setReward(rewardFileName);
        vector<string> activities = {
            act1, act2
        };
        scene1.setActivities(activities);

        // Objects should probably just consist of string fileNames to
        // images of the objects and Scenes should have a vector of
        // of floats of structs for the width, height, and position of objects.
        // But I'm just keeping the Scene class easy to understand right now.
        vector<string> objects = {
            obj1, obj2, obj3
        };
        scene1.setObjects(objects);
        scene1.saveToFile("db/scenes/river.cereal");

        WHEN("Another Scene object loads from that file")
        {
            Scene scene2("db/scenes/river.cereal", 640, 480);

		    THEN("The second scene contains all of the data entered into the first scene")
		    {
		        REQUIRE(scene2.getBackground().getPosition().x == 0);
		        REQUIRE(scene2.getBackground().getPosition().y == 0);
		        REQUIRE(scene2.getBGM()[0]->getVolume() == volume);
		        REQUIRE(scene2.getBGM()[0]->getPitch() == pitch);
		        REQUIRE(scene2.getBGMStr()[0] == BGMfileName);
		        REQUIRE(scene2.getBGIStr() == BGIfileName);
		        REQUIRE(scene2.getRequirement() == reqFileName);
		        REQUIRE(scene2.getReward() == rewardFileName);
			}
        }

	    WHEN("getBGM() is called")
	    {
	        const string* bgm = &test_screen.getBGM();

	        THEN("getBGM() should return an empty string")
		    {
			    REQUIRE(*bgm == "");
		    }
	    }
	}
}

*/
