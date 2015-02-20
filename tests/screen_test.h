// Screen_test.cpp
// Shaun Bond
// 2/15/2014
//
// Unit test file for the Screen class

#include "screen.h"
#include "catch.hpp"

SCENARIO("Screen Class", "[Screen_class]")
{
    GIVEN("A newly constructed Screen object")
    {
        Screen test_screen;

		WHEN("getScreenItems() is called")
        {
		    const vector<ScreenItem> si = test_screen.getScreenItems();

		    THEN("It should return an empty vector")
		    {
		        REQUIRE(si.empty() == true);
			}
        }

	    WHEN("getBGM() is called")
	    {
	        const vector<MusicItem>* bgm = &test_screen.getBGM();

	        THEN("getBGM() should return an empty vector")
		    {
			    REQUIRE(bgm->empty() == true);
		    }
	    }

	GIVEN("A vector of ScreenItems")
	{
        vector<ScreenItem> vec;

        ScreenItem si;
	    si.type = IMAGE;
	    si.name = L"fakeimage.png";
	    si.size = Vec2(640,480);
	    si.position = Vec2(.5,.5);
	    vec.push_back(si);

	    si.type = TEXT;
        si.name = L"faketext";
        si.size = Vec2(40,80);
        si.position = Vec2(.1,.5);
        vec.push_back(si);

        si.type = HOTSPOT;
        si.sound = L"fakesound.mpg";
        si.size = Vec2(10,10);
        si.position = Vec2(.5,.1);
        si.volume = 59;
        si.pitch = 100;
        vec.push_back(si);

	    WHEN("setScreenItems is called with that vector")
        {
	        test_screen.setScreenItems(vec);

	        THEN("getScreenItems should return the same vector")
	        {
	            REQUIRE(test_screen.getScreenItems() == vec);
	        }
        }
	}
	}
}


