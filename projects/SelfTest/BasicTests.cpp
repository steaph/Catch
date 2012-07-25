/*
 *  BasicTests.cpp
 *  Catch - Test
 *
 *  Created by Martin  on 25/07/2012.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include "catch.hpp"
#include "catch_self_test.hpp"

#include <iostream>
#include <string>

TEST_CASE( "./succeeding/Basic/integral", "integral comparison" )
{
    REQUIRE( 1 == 1 );
    REQUIRE( 1 != 2 );
    
    int a = 1;
    int b = 2;
    
    REQUIRE( a != b );
    REQUIRE( b != a );
}

TEST_CASE( "./failing/Basic/integral", "integral comparison" )
{
    REQUIRE( 1 != 1 );
    REQUIRE( 1 == 2 );
    
    int a = 1;
    int b = 2;
    
    REQUIRE( a == b );
    REQUIRE( b == a );
}

TEST_CASE( "./succeeding/Basic/cstring", "C string comparison" )
{
//    REQUIRE( "hello" == "hello" );
//    REQUIRE( "hello" != "world" );
    
    char * h = "hello" ;
    char * w = "world" ;
    
    REQUIRE( h == h );
    REQUIRE( w == w );
    REQUIRE( h != w );
    REQUIRE( w != h );

    char const * const ch = "hello" ;
    char const * const cw= "world" ;
    
    REQUIRE( ch == ch );
    REQUIRE( cw == cw );
    REQUIRE( ch != cw );
    REQUIRE( cw != ch );

    REQUIRE( ch ==  h );
    REQUIRE(  h == ch );
    REQUIRE( cw ==  w );
    REQUIRE(  w == cw );
}

TEST_CASE( "./failing/Basic/cstring", "C string comparison" )
{
    REQUIRE( "hello" != "hello" );
    REQUIRE( "hello" == "world" );
    
    char * h = "hello" ;
    char * w = "world" ;
    
    REQUIRE( h == w );
    REQUIRE( w == h );
}

TEST_CASE( "./succeeding/Basic/string", "std::string comparison" )
{
    REQUIRE( std::string("hello") == std::string("hello") );
    REQUIRE( std::string("hello") != std::string("world") );
    
    std::string h = "hello" ;
    std::string w = "world" ;
    
    REQUIRE( h != w );
    REQUIRE( w != h );
}

TEST_CASE( "./failing/Basic/string", "std::string comparison" )
{
    REQUIRE( std::string("hello") != std::string("hello") );
    REQUIRE( std::string("hello") == std::string("world") );
    
    std::string h = "hello" ;
    std::string w = "world" ;
    
    REQUIRE( h == w );
    REQUIRE( w == h );
}

TEST_CASE( "./succeeding/Basic/mixedstring", "C and std::string comparison" )
{
    REQUIRE( std::string("hello") == "hello" );
    REQUIRE( std::string("hello") != "world" );
    REQUIRE( "hello" == std::string("hello") );
    REQUIRE( "hello" != std::string("world") );
    
    char * ch = "hello" ;
    char * cw = "world" ;
    std::string h = "hello" ;
    std::string w = "world" ;
    
    REQUIRE( h != cw );
    REQUIRE( w != ch );
    REQUIRE( ch != w );
    REQUIRE( cw != h );
}

TEST_CASE( "./failing/Basic/mixedstring", "C and std::string comparison" )
{
    REQUIRE( std::string("hello") != "hello" );
    REQUIRE( std::string("hello") == "world" );
    REQUIRE( "hello" != std::string("hello") );
    REQUIRE( "hello" == std::string("world") );
    
    char * ch = "hello" ;
    char * cw = "world" ;
    std::string h = "hello" ;
    std::string w = "world" ;
    
    REQUIRE( h == cw );
    REQUIRE( w == ch );
    REQUIRE( ch == w );
    REQUIRE( cw == h );
}
