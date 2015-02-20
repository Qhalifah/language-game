// The master test suite, should include all unit test files here.

// This tells Catch to provide a main() - only do this in one cpp file
// Be sure not to compile this file along with main.cpp or errors will ensue
// This means one or the other may have to be omited from the build in your ide
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "screen_test.h"
#include "dictionary_test.h"
#include "profile_test.h"
#include "scene_test.h"

// This is the template for writing tests
// Write these in a separate header file for each cpp file
// Something like "cppname_test.h"
// Then we can easily include/remove them here

/*
SCENARIO( "vectors can be sized and resized", "[vector]" ) {

    GIVEN( "A vector with some items" ) {
        std::vector<int> v( 5 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );

        WHEN( "the size is increased" ) {
            v.resize( 10 );

            THEN( "the size and capacity change" ) {
                REQUIRE( v.size() == 10 );
                REQUIRE( v.capacity() >= 10 );
            }
        }
        WHEN( "the size is reduced" ) {
            v.resize( 0 );

            THEN( "the size changes but not capacity" ) {
                REQUIRE( v.size() == 0 );
                REQUIRE( v.capacity() >= 5 );
            }
        }
        WHEN( "more capacity is reserved" ) {
            v.reserve( 10 );

            THEN( "the capacity changes but not the size" ) {
                REQUIRE( v.size() == 5 );
                REQUIRE( v.capacity() >= 10 );
            }
        }
        WHEN( "less capacity is reserved" ) {
            v.reserve( 0 );

            THEN( "neither size nor capacity are changed" ) {
                REQUIRE( v.size() == 5 );
                REQUIRE( v.capacity() >= 5 );
            }
        }
    }
}*/
