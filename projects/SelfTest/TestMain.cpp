/*
 *  Created by Phil on 22/10/2010.
 *  Copyright 2010 Two Blue Cubes Ltd
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#endif

#include "catch_self_test.hpp"
#include "internal/catch_line_wrap.h"

#define CATCH_DIMENSION_OF( a ) ( sizeof(a) / sizeof(0[a]) )

TEST_CASE( "selftest/main", "Runs all Catch self tests and checks their results" ) {
    using namespace Catch;

    ///////////////////////////////////////////////////////////////////////////
    SECTION(    "selftest/expected result",
                "Tests do what they claim" ) {

        SECTION(    "selftest/expected result/failing tests",
                    "Tests in the 'failing' branch fail" ) {
            MetaTestRunner::runMatching( "./failing/*",  MetaTestRunner::Expected::ToFail, 0, 2 );
        }

        SECTION(    "selftest/expected result/succeeding tests",
                    "Tests in the 'succeeding' branch succeed" ) {
            MetaTestRunner::runMatching( "./succeeding/*",  MetaTestRunner::Expected::ToSucceed, 1, 2 );
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    SECTION(    "selftest/test counts",
                "Number of test cases that run is fixed" ) {
        EmbeddedRunner runner;

        SECTION(    "selftest/test counts/succeeding tests",
                    "Number of 'succeeding' tests is fixed" ) {
            Totals totals = runner.runMatching( "./succeeding/*", 0, 2 );
            CHECK( totals.assertions.passed == 378 );
            CHECK( totals.assertions.failed == 0 );
        }

        SECTION(    "selftest/test counts/failing tests",
                    "Number of 'failing' tests is fixed" ) {
            Totals totals = runner.runMatching( "./failing/*", 1, 2 );
            CHECK( totals.assertions.passed == 1 );
            CHECK( totals.assertions.failed == 101 );
        }
    }
}

TEST_CASE( "meta/Misc/Sections", "looped tests" ) {
    Catch::EmbeddedRunner runner;

    Catch::Totals totals = runner.runMatching( "./mixed/Misc/Sections/nested2", 0, 1 );
    CHECK( totals.assertions.passed == 2 );
    CHECK( totals.assertions.failed == 1 );
}

#ifdef __clang__
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

#include "../../include/internal/catch_commandline.hpp"
#include "../../include/internal/catch_test_spec.h"
#include "../../include/reporters/catch_reporter_basic.hpp"
#include "../../include/reporters/catch_reporter_xml.hpp"
#include "../../include/reporters/catch_reporter_junit.hpp"

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
template<size_t size>
void parseIntoConfig( const char * (&argv)[size], Catch::ConfigData& config ) {
#else
void parseIntoConfig( size_t size, const char * argv[], Catch::ConfigData& config ) {
#endif
    static Catch::AllOptions options;
    options.parseIntoConfig( Catch::CommandParser( size, argv ), config );
}

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
template<size_t size>
std::string parseIntoConfigAndReturnError( const char * (&argv)[size], Catch::ConfigData& config ) {
#else
std::string parseIntoConfigAndReturnError( size_t size, const char * argv[], Catch::ConfigData& config ) {
#endif
    try {
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
        parseIntoConfig( argv, config );
#else
        parseIntoConfig( size, argv, config );
#endif
        FAIL( "expected exception" );
    }
    catch( std::exception& ex ) {
        return ex.what();
    }
    return "";
}

inline Catch::TestCase fakeTestCase( const char* name ){ return Catch::makeTestCase( NULL, "", name, "", CATCH_INTERNAL_LINEINFO ); }

TEST_CASE( "selftest/parser/2", "ConfigData" ) {

    Catch::ConfigData config;

    SECTION( "default", "" ) {
        const char* argv[] = { "test" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
        CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
        CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

        CHECK( config.shouldDebugBreak == false );
        CHECK( config.cutoff == -1 );
        CHECK( config.allowThrows == true );
        CHECK( config.reporter.empty() );
    }

    SECTION( "test lists", "" ) {
        SECTION( "-t/1", "Specify one test case using -t" ) {
            const char* argv[] = { "test", "-t", "test1" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "notIncluded" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) );
        }
        SECTION( "-t/exclude:1", "Specify one test case exclusion using -t exclude:" ) {
            const char* argv[] = { "test", "-t", "exclude:test1" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "alwaysIncluded" ) ) );
        }

        SECTION( "--test/1", "Specify one test case using --test" ) {
            const char* argv[] = { "test", "--test", "test1" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "notIncluded" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) );
        }

        SECTION( "--test/exclude:1", "Specify one test case exclusion using --test exclude:" ) {
            const char* argv[] = { "test", "--test", "exclude:test1" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "alwaysIncluded" ) ) );
        }

        SECTION( "--test/exclude:2", "Specify one test case exclusion using --test ~" ) {
            const char* argv[] = { "test", "--test", "~test1" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "alwaysIncluded" ) ) );
        }

        SECTION( "-t/2", "Specify two test cases using -t" ) {
            const char* argv[] = { "test", "-t", "test1", "test2" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.filters.size() == 1 );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "notIncluded" ) ) == false );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) );
            REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test2" ) ) );
        }

        SECTION( "-t/0", "When no test names are supplied it is an error" ) {
            const char* argv[] = { "test", "-t" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "at least 1" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "at least 1" ) );
#endif
        }
    }

    SECTION( "reporter", "" ) {
        SECTION( "-r/console", "" ) {
            const char* argv[] = { "test", "-r", "console" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.reporter == "console" );
        }
        SECTION( "-r/xml", "" ) {
            const char* argv[] = { "test", "-r", "xml" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.reporter == "xml" );
        }
        SECTION( "--reporter/junit", "" ) {
            const char* argv[] = { "test", "--reporter", "junit" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.reporter == "junit" );
        }
        SECTION( "-r/error", "reporter config only accepts one argument" ) {
            const char* argv[] = { "test", "-r", "one", "two" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "1 argument" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "1 argument" ) );
#endif
        }
    }

    SECTION( "debugger", "" ) {
        SECTION( "-b", "" ) {
            const char* argv[] = { "test", "-b" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.shouldDebugBreak == true );
        }
        SECTION( "--break", "" ) {
            const char* argv[] = { "test", "--break" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.shouldDebugBreak );
        }
        SECTION( "-b", "break option has no arguments" ) {
            const char* argv[] = { "test", "-b", "unexpected" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "0 arguments" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "0 arguments" ) );
#endif
        }
    }

    SECTION( "abort", "" ) {
        SECTION( "-a", "" ) {
            const char* argv[] = { "test", "-a" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.cutoff == 1 );
        }
        SECTION( "-a/2", "" ) {
            const char* argv[] = { "test", "-a", "2" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.cutoff == 2 );
        }
        SECTION( "-a/error/0", "" ) {
            const char* argv[] = { "test", "-a", "0" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "greater than zero" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "greater than zero" ) );
#endif
        }
        SECTION( "-a/error/non numeric", "" ) {
            const char* argv[] = { "test", "-a", "oops" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "greater than zero" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "greater than zero" ) );
#endif
        }
        SECTION( "-a/error/two args", "cutoff only takes one argument" ) {
            const char* argv[] = { "test", "-a", "1", "2" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            REQUIRE_THAT( parseIntoConfigAndReturnError( argv, config ), Contains( "0 and 1 argument" ) );
#else
            REQUIRE_THAT( parseIntoConfigAndReturnError( CATCH_DIMENSION_OF(argv), argv, config ), Contains( "0 and 1 argument" ) );
#endif
        }
    }

    SECTION( "nothrow", "" ) {
        SECTION( "-nt", "" ) {
            const char* argv[] = { "test", "-nt" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.allowThrows == false );
        }
        SECTION( "--nothrow", "" ) {
            const char* argv[] = { "test", "--nothrow" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            REQUIRE( config.allowThrows == false );
        }
    }

    SECTION( "streams", "" ) {
        SECTION( "-o filename", "" ) {
            const char* argv[] = { "test", "-o", "filename.ext" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif
            REQUIRE( config.outputFilename == "filename.ext" );
            REQUIRE( config.stream.empty() );
        }
        SECTION( "-o %stdout", "" ) {
            const char* argv[] = { "test", "-o", "%stdout" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif
            REQUIRE( config.stream == "stdout" );
            REQUIRE( config.outputFilename.empty() );
        }
        SECTION( "--out", "" ) {
            const char* argv[] = { "test", "--out", "filename.ext" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif
            REQUIRE( config.outputFilename == "filename.ext" );
        }
    }

    SECTION( "combinations", "" ) {
        SECTION( "-a -b", "" ) {
            const char* argv[] = { "test", "-a", "-b", "-nt" };
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
            CHECK_NOTHROW( parseIntoConfig( argv, config ) );
#else
            CHECK_NOTHROW( parseIntoConfig( CATCH_DIMENSION_OF(argv), argv, config ) );
#endif

            CHECK( config.cutoff == 1 );
            CHECK( config.shouldDebugBreak );
            CHECK( config.allowThrows == false );
        }
    }
}

TEST_CASE( "selftest/test filter", "Individual filters" ) {

    Catch::TestCaseFilter matchAny( "*" );
    Catch::TestCaseFilter matchNone( "*", Catch::IfFilterMatches::ExcludeTests );
    CHECK( matchAny.shouldInclude( fakeTestCase( "any" ) ));
    CHECK( matchNone.shouldInclude( fakeTestCase( "any" ) ) == false );

    Catch::TestCaseFilter matchHidden( "./*" );
    Catch::TestCaseFilter matchNonHidden( "./*", Catch::IfFilterMatches::ExcludeTests );

    CHECK( matchHidden.shouldInclude( fakeTestCase( "any" ) ) == false );
    CHECK( matchNonHidden.shouldInclude( fakeTestCase( "any" ) ) );

    CHECK( matchHidden.shouldInclude( fakeTestCase( "./any" ) ) );
    CHECK( matchNonHidden.shouldInclude( fakeTestCase( "./any" ) ) == false );
}

TEST_CASE( "selftest/test filters", "Sets of filters" ) {

    Catch::TestCaseFilter matchHidden( "./*" );
    Catch::TestCaseFilter dontMatchA( "./a*", Catch::IfFilterMatches::ExcludeTests );
    Catch::TestCaseFilters filters( "" );
    filters.addFilter( matchHidden );
    filters.addFilter( dontMatchA );

    CHECK( matchHidden.shouldInclude( fakeTestCase( "./something" ) ) );

    CHECK( filters.shouldInclude( fakeTestCase( "any" ) ) == false );
    CHECK( filters.shouldInclude( fakeTestCase( "./something" ) ) );
    CHECK( filters.shouldInclude( fakeTestCase( "./anything" ) ) == false );
}

TEST_CASE( "selftest/filter/prefix wildcard", "Individual filters with wildcards at the start" ) {
    Catch::TestCaseFilter matchBadgers( "*badger" );

    CHECK( matchBadgers.shouldInclude( fakeTestCase( "big badger" ) ));
    CHECK( matchBadgers.shouldInclude( fakeTestCase( "little badgers" ) ) == false );
}
TEST_CASE( "selftest/filter/wildcard at both ends", "Individual filters with wildcards at both ends" ) {
    Catch::TestCaseFilter matchBadgers( "*badger*" );

    CHECK( matchBadgers.shouldInclude( fakeTestCase( "big badger" ) ));
    CHECK( matchBadgers.shouldInclude( fakeTestCase( "little badgers" ) ) );
    CHECK( matchBadgers.shouldInclude( fakeTestCase( "badgers are big" ) ) );
    CHECK( matchBadgers.shouldInclude( fakeTestCase( "hedgehogs" ) ) == false );
}


#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
template<size_t size>
int getArgc( const char * (&)[size] ) {
    return size;
}
#endif

TEST_CASE( "selftest/option parsers", "" )
{
    Catch::ConfigData config;

    Catch::SharedImpl<Catch::Options::TestCaseOptionParser> tcOpt;
    Catch::OptionParser& opt = tcOpt;

    const char* argv[] = { "test", "-t", "test1" };

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
    Catch::CommandParser parser( getArgc( argv ), argv );
#else
    Catch::CommandParser parser( CATCH_DIMENSION_OF(argv), argv );
#endif

    CHECK_NOTHROW( opt.parseIntoConfig( parser, config ) );

    REQUIRE( config.filters.size() == 1 );
    REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "notIncluded" ) ) == false );
    REQUIRE( config.filters[0].shouldInclude( fakeTestCase( "test1" ) ) );
}

TEST_CASE( "selftest/tags", "" ) {

    std::string p1 = "[one]";
    std::string p2 = "[one],[two]";
    std::string p3 = "[one][two]";
    std::string p4 = "[one][two],[three]";
    std::string p5 = "[one][two]~[hide],[three]";

    SECTION( "one tag", "" ) {
        Catch::TestCase oneTag = Catch::makeTestCase( NULL, "", "test", "[one]", CATCH_INTERNAL_LINEINFO );

        CHECK( oneTag.getTestCaseInfo().description == "" );
        CHECK( oneTag.hasTag( "one" ) );
        CHECK( oneTag.getTags().size() == 1 );

        CHECK( oneTag.matchesTags( p1 ) == true );
        CHECK( oneTag.matchesTags( p2 ) == true );
        CHECK( oneTag.matchesTags( p3 ) == false );
        CHECK( oneTag.matchesTags( p4 ) == false );
        CHECK( oneTag.matchesTags( p5 ) == false );
    }

    SECTION( "two tags", "" ) {
        Catch::TestCase twoTags= Catch::makeTestCase( NULL, "", "test", "[one][two]", CATCH_INTERNAL_LINEINFO );

        CHECK( twoTags.getTestCaseInfo().description == "" );
        CHECK( twoTags.hasTag( "one" ) );
        CHECK( twoTags.hasTag( "two" ) );
        CHECK( twoTags.hasTag( "Two" ) );
        CHECK( twoTags.hasTag( "three" ) == false );
        CHECK( twoTags.getTags().size() == 2 );

        CHECK( twoTags.matchesTags( p1 ) == true );
        CHECK( twoTags.matchesTags( p2 ) == true );
        CHECK( twoTags.matchesTags( p3 ) == true );
        CHECK( twoTags.matchesTags( p4 ) == true );
        CHECK( twoTags.matchesTags( p5 ) == true );
    }

    SECTION( "one tag with characters either side", "" ) {

        Catch::TestCase oneTagWithExtras = Catch::makeTestCase( NULL, "", "test", "12[one]34", CATCH_INTERNAL_LINEINFO );
        CHECK( oneTagWithExtras.getTestCaseInfo().description == "1234" );
        CHECK( oneTagWithExtras.hasTag( "one" ) );
        CHECK( oneTagWithExtras.hasTag( "two" ) == false );
        CHECK( oneTagWithExtras.getTags().size() == 1 );
    }

    SECTION( "start of a tag, but not closed", "" ) {

        Catch::TestCase oneTagOpen = Catch::makeTestCase( NULL, "", "test", "[one", CATCH_INTERNAL_LINEINFO );

        CHECK( oneTagOpen.getTestCaseInfo().description == "[one" );
        CHECK( oneTagOpen.hasTag( "one" ) == false );
        CHECK( oneTagOpen.getTags().size() == 0 );
    }

    SECTION( "hidden", "" ) {
        Catch::TestCase oneTag = Catch::makeTestCase( NULL, "", "test", "[hide]", CATCH_INTERNAL_LINEINFO );

        CHECK( oneTag.getTestCaseInfo().description == "" );
        CHECK( oneTag.hasTag( "hide" ) );
        CHECK( oneTag.isHidden() );

        CHECK( oneTag.matchesTags( "~[hide]" ) == false );

    }
}

TEST_CASE( "Long strings can be wrapped", "[wrap]" ) {

    SECTION( "plain string", "" ) {
        // guide:                 123456789012345678
        std::string testString = "one two three four";
        
        SECTION( "No wrapping", "" ) {
            CHECK( Catch::LineWrapper().setRight( 80 ).wrap( testString ).toString() == testString );
            CHECK( Catch::LineWrapper().setRight( 18 ).wrap( testString ).toString() == testString );
        }
        SECTION( "Wrapped once", "" ) {
            CHECK( Catch::LineWrapper().setRight( 17 ).wrap( testString ).toString() == "one two three\nfour" );
            CHECK( Catch::LineWrapper().setRight( 16 ).wrap( testString ).toString() == "one two three\nfour" );
            CHECK( Catch::LineWrapper().setRight( 15 ).wrap( testString ).toString() == "one two three\nfour" );
            CHECK( Catch::LineWrapper().setRight( 14 ).wrap( testString ).toString() == "one two three\nfour" );
            CHECK( Catch::LineWrapper().setRight( 13 ).wrap( testString ).toString() == "one two\nthree four" );
        }
        SECTION( "Wrapped twice", "" ) {
            CHECK( Catch::LineWrapper().setRight( 9 ).wrap( testString ).toString() == "one two\nthree\nfour" );
            CHECK( Catch::LineWrapper().setRight( 8 ).wrap( testString ).toString() == "one two\nthree\nfour" );
        }
        SECTION( "Wrapped three times", "" ) {
            CHECK( Catch::LineWrapper().setRight( 7 ).wrap( testString ).toString() == "one\ntwo\nthree\nfour" );
            CHECK( Catch::LineWrapper().setRight( 5 ).wrap( testString ).toString() == "one\ntwo\nthree\nfour" );
        }
        SECTION( "Short wrap", "" ) {
            CHECK( Catch::LineWrapper().setRight( 4 ).wrap( "abcdef" ).toString() == "abc-\ndef" );
            CHECK( Catch::LineWrapper().setRight( 4 ).wrap( "abcdefg" ).toString() == "abc-\ndefg" );
            CHECK( Catch::LineWrapper().setRight( 4  ).wrap("abcdefgh" ).toString() == "abc-\ndef-\ngh" );

            CHECK( Catch::LineWrapper().setRight( 4 ).wrap( testString ).toString() == "one\ntwo\nthr-\nee\nfour" );
            CHECK( Catch::LineWrapper().setRight( 3 ).wrap( testString ).toString() == "one\ntwo\nth-\nree\nfo-\nur" );
        }
        SECTION( "As container", "" ) {
            Catch::LineWrapper wrapper;
            wrapper.setRight( 7 ).wrap( testString );
            CHECK( wrapper.size() == 4 );
            CHECK( wrapper[0] == "one" );
            CHECK( wrapper[1] == "two" );
            CHECK( wrapper[2] == "three" );
            CHECK( wrapper[3] == "four" );
        }
    }
    
    SECTION( "With newlines", "" ) {
        
        // guide:                 1234567890123456789
        std::string testString = "one two\nthree four";
        
        SECTION( "No wrapping" , "" ) {
            CHECK( Catch::LineWrapper().setRight( 80 ).wrap( testString ).toString() == testString );
            CHECK( Catch::LineWrapper().setRight( 18 ).wrap( testString ).toString() == testString );
            CHECK( Catch::LineWrapper().setRight( 10 ).wrap( testString ).toString() == testString );
        }
        SECTION( "Trailing newline" , "" ) {
            CHECK( Catch::LineWrapper().setRight( 10 ).wrap( "abcdef\n" ).toString() == "abcdef\n" );
            CHECK( Catch::LineWrapper().setRight(  6 ).wrap( "abcdef" ).toString() == "abcdef" );
            CHECK( Catch::LineWrapper().setRight(  6 ).wrap( "abcdef\n" ).toString() == "abcdef\n" );
        }
        SECTION( "Wrapped once", "" ) {
            CHECK( Catch::LineWrapper().setRight( 9 ).wrap( testString ).toString() == "one two\nthree\nfour" );
            CHECK( Catch::LineWrapper().setRight( 8 ).wrap( testString ).toString() == "one two\nthree\nfour" );
            CHECK( Catch::LineWrapper().setRight( 7 ).wrap( testString ).toString() == "one two\nthree\nfour" );
        }
        SECTION( "Wrapped twice", "" ) {
            CHECK( Catch::LineWrapper().setRight( 6 ).wrap( testString ).toString() == "one\ntwo\nthree\nfour" );
        }
    }
    
}

using namespace Catch;

class ColourString {
public:

    struct ColourIndex {
        ColourIndex( Colour::Code _colour, std::size_t _fromIndex, std::size_t _toIndex )
        :   colour( _colour ),
            fromIndex( _fromIndex ),
            toIndex( _toIndex )
        {}

        Colour::Code colour;
        std::size_t fromIndex;
        std::size_t toIndex;
    };
    
    ColourString( std::string const& _string )
    : string( _string )
    {}
    ColourString( std::string const& _string, std::vector<ColourIndex> const& _colours )
    : string( _string ), colours( _colours )
    {}
    
    ColourString& addColour( Colour::Code colour, int _index ) {
        colours.push_back( ColourIndex( colour,
                                        resolveRelativeIndex( _index ),
                                        resolveRelativeIndex( _index )+1 ) );
        return *this;
    }
    ColourString& addColour( Colour::Code colour, int _fromIndex, int _toIndex ) {
        colours.push_back( ColourIndex( colour,
                                        resolveRelativeIndex(_fromIndex),
                                        resolveLastRelativeIndex( _toIndex ) ) );
        return *this;
    }
    
    void writeToStream( std::ostream& _stream ) const {
        std::size_t last = 0;
        for( std::size_t i = 0; i < colours.size(); ++i ) {
            ColourIndex const& index = colours[i];
            if( index.fromIndex > last )
                _stream << string.substr( last, index.fromIndex-last );
            {
                Colour colourGuard( index.colour );
                _stream << string.substr( index.fromIndex, index.toIndex-index.fromIndex );
            }
            last = index.toIndex;
        }
        if( last < string.size() )
            _stream << string.substr( last );        
    }
    friend std::ostream& operator << ( std::ostream& _stream, ColourString const& _colourString ) {
        _colourString.writeToStream( _stream );
        return _stream;
    }

private:

    std::size_t resolveLastRelativeIndex( int _index ) {
        std::size_t index = resolveRelativeIndex( _index );
        return index == 0 ? string.size() : index;
    }
    std::size_t resolveRelativeIndex( int _index ) {
        return static_cast<std::size_t>( _index >= 0
            ? _index
            : static_cast<int>( string.size() )+_index );
    }
    std::string string;
    std::vector<ColourIndex> colours;
};

class Text
{
public:
    Text( std::string const& _string ) : originalString( _string ) {
    }
    Text( char const* const _string ) : originalString( _string ) {
    }
    
    friend std::ostream& operator << ( std::ostream& _stream, Text const& _text ) {
        _text.print( _stream );
        return _stream;
    }

private:
    void process() const {
        
    }
    void print( std::ostream& stream ) const {
        stream << originalString;
    }
    
    std::string originalString;
//    std::vector<
};

TEST_CASE( "Strings can be rendered with colour", "[colour]" ) {
//    Text text = "`red`This is in red. `green` this is in green";
//    std::cout << text << std::endl;
    
    {
        ColourString cs( "hello" );
        cs  .addColour( Colour::Red, 0 )
            .addColour( Colour::Green, -1 );

        std::cout << cs << std::endl;
    }

    {
        ColourString cs( "hello" );
        cs  .addColour( Colour::Blue, 1, -2 );
        
        std::cout << cs << std::endl;
    }
    
}
