// Renderer_test.cpp
// Shaun Bond
// 2/15/2014
//
// Unit test file for the Renderer class

#include "interface.h"
#include "catch.hpp"
//#include "SFML/Graphics.hpp"


SCENARIO("Renderer class", "[Renderer_class]")
{
    GIVEN("A Renderer object")
    {
        Renderer renderer;

        WHEN("Render is called with an empty Screen")
        {
            Screen screen;
            renderer.render(screen);

            THEN("The renderer should do nothing")
            {

            }
        }

        WHEN("")
        {
            THEN("")
            {

            }
        }

        WHEN("")
        {
            THEN("")
            {

            }
        }
    }
}

