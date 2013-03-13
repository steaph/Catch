/*
 *  Created by Phil on 09/11/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#endif

#include "catch.hpp"

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

#define VC6_SUCCEED_TEST( tag, N ) \
    TEST_CASE( tag, "VC6: Skipping tricky tests" ) {\
        WARN( "VC6: Skip succeeding tricky test:" ); \
        for ( int i = 0; i < N; ++i ) { \
            SUCCEED( "VC6: Skip succeeding tricky test." ); \
        } \
    }

#define VC6_FAIL_TEST( tag, N ) \
    TEST_CASE( tag, "VC6: Skipping tricky tests" ) { \
        for ( int i = 0; i < N; ++i ) { \
            CHECK( 0 == "VC6: Skip failing tricky test." ); \
        } \
    }

#endif

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )
#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

VC6_SUCCEED_TEST( "./succeeding/Tricky/std::pair", 1 )

#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

namespace Catch
{
    template<>
    std::string toString<std::pair<int, int> >( const std::pair<int, int>& value )
    {
        std::ostringstream oss;
        oss << "std::pair( " << value.first << ", " << value.second << " )";
        return oss.str();

    }
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE
(
    "./succeeding/Tricky/std::pair",
    "Parsing a std::pair"
)
{
    std::pair<int, int> aNicePair( 1, 2 );

    // !TBD: would be nice if this could compile without the extra parentheses
    REQUIRE( (std::pair<int, int>( 1, 2 )) == aNicePair );

}
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

///////////////////////////////////////////////////////////////////////////////
TEST_CASE
(
    "./inprogress/failing/Tricky/trailing expression",
    "Where the is more to the expression after the RHS"
)
{
    /*
    int a = 1;
    int b = 2;

    // This only captures part of the expression, but issues a warning about the rest
    REQUIRE( a == 2 || b == 2 );
     */
    WARN( "Uncomment the code in this test to check that it gives a sensible compiler error" );
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE
(
    "./inprogress/failing/Tricky/compound lhs",
    "Where the LHS is not a simple value"
)
{
    /*
    int a = 1;
    int b = 2;

    // This only captures part of the expression, but issues a warning about the rest
    REQUIRE( a+1 == b-1 );
    */
    WARN( "Uncomment the code in this test to check that it gives a sensible compiler error" );
}

///////////////////////////////////////////////////////////////////////////////
#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

VC6_FAIL_TEST( "./failing/Tricky/non streamable type", 2 )

#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

struct Opaque
{
    int val;
    bool operator ==( const Opaque& o ) const
    {
        return val == o.val;
    }
};

TEST_CASE
(
    "./failing/Tricky/non streamable type",
    "A failing expression with a non streamable type is still captured"
)
{

    Opaque o1, o2;
    o1.val = 7;
    o2.val = 8;

    CHECK( &o1 == &o2 );
    CHECK( o1 == o2 );
}
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

///////////////////////////////////////////////////////////////////////////////
TEST_CASE
(
    "./failing/string literals",
    "string literals of different sizes can be compared"
)
{
    REQUIRE( std::string( "first" ) == "second" );
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE
(
    "./succeeding/side-effects",
    "An expression with side-effects should only be evaluated once"
)
{
    int i = 7;

    REQUIRE( i++ == 7 );
    REQUIRE( i++ == 8 );

}

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

// VC6_SUCCEED_TEST( "./succeeding/koenig, 0 )

#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

namespace A {
    struct X
    {
        X() : a(4), b(2), c(7) {}
        X(int v) : a(v), b(2), c(7) {}
        int a;
        int b;
        int c;
    };
}

namespace B {
    struct Y
    {
        Y() : a(4), b(2), c(7) {}
        Y(int v) : a(v), b(2), c(7) {}
        int a;
        int b;
        int c;
    };
}

inline bool operator==(const A::X& lhs, const B::Y& rhs)
{
    return (lhs.a == rhs.a);
}

inline bool operator==(const B::Y& lhs, const A::X& rhs)
{
    return (lhs.a == rhs.a);
}


///////////////////////////////////////////////////////////////////////////////
/* This, currently, does not compile with LLVM
TEST_CASE
(
    "./succeeding/koenig",
    "Operators at different namespace levels not hijacked by Koenig lookup"
)
{
    A::X x;
    B::Y y;
    REQUIRE( x == y );
}
*/
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

namespace ObjectWithConversions
{
    VC6_SUCCEED_TEST( "./succeeding/koenig", 1 )
}
namespace ObjectWithNonConstEqualityOperator
{
    VC6_SUCCEED_TEST( "./succeeding/non-const==", 1 )
}
namespace EnumBitFieldTests
{
    VC6_SUCCEED_TEST( "./succeeding/enum/bits", 1 )
}

#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

namespace ObjectWithConversions
{
    struct Object
    {
        operator unsigned int() {return 0xc0000000;}
    };

    ///////////////////////////////////////////////////////////////////////////////
    TEST_CASE
    (
        "./succeeding/koenig",
        "Operators at different namespace levels not hijacked by Koenig lookup"
    )
    {
        Object o;
        REQUIRE(0xc0000000 == o );
    }
}

namespace ObjectWithNonConstEqualityOperator
{
    struct Test
    {
        Test( unsigned int v )
        : m_value(v)
        {}

        bool operator==( const Test&rhs )
        {
            return (m_value == rhs.m_value);
        }
        bool operator==( const Test&rhs ) const
        {
            return (m_value != rhs.m_value);
        }
        unsigned int m_value;
    };

    TEST_CASE("./succeeding/non-const==", "Demonstrate that a non-const == is not used")
    {
        Test t( 1 );
        REQUIRE( t == 1u );
    }
}

namespace EnumBitFieldTests
{
    enum Bits {bit0 = 0x0001, bit1 = 0x0002, bit2 = 0x0004, bit3 = 0x0008, bit1and2 = 0x0006,
        bit30 = 0x40000000, bit31 = 0x80000000,
        bit30and31 = 0xc0000000};

    TEST_CASE("./succeeding/enum/bits", "Test enum bit values")
    {
        REQUIRE( 0xc0000000 == bit30and31 );
    }
}
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

struct Obj
{
    Obj():prop(&p){}

    int p;
    int* prop;
};

TEST_CASE("./succeeding/boolean member", "")
{
    Obj obj;
    REQUIRE( obj.prop != NULL );
}

#if defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

VC6_SUCCEED_TEST( "./succeeding/unimplemented static bool", 8 )

#else // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

// Tests for a problem submitted by Ralph McArdell
//
// The static bool value should not need to be defined outside the
// struct it is declared in - but when evaluating it in a deduced
// context it appears to require the extra definition.
// The issue was fixed by adding bool overloads to bypass the
// templates that were deduce it.
template <bool B>
struct is_true
{
    static const bool value = B;
};

TEST_CASE( "./succeeding/unimplemented static bool", "static bools can be evaluated" )
{
    SECTION("compare to true","")
    {
        REQUIRE( is_true<true>::value == true );
        REQUIRE( true == is_true<true>::value );
    }
    SECTION("compare to false","")
    {
        REQUIRE( is_true<false>::value == false );
        REQUIRE( false == is_true<false>::value );
    }

    SECTION("negation", "")
    {
        REQUIRE( !is_true<false>::value );
    }

    SECTION("double negation","")
    {
        REQUIRE( !!is_true<true>::value );
    }

    SECTION("direct","")
    {
        REQUIRE( is_true<true>::value );
        REQUIRE_FALSE( is_true<false>::value );
    }
}
#endif  // defined( INTERNAL_CATCH_COMPILER_IS_MSVC6 )

// Uncomment these tests to produce an error at test registration time
/*
TEST_CASE( "./sameName", "Tests with the same name are not allowed" )
{

}
TEST_CASE( "./sameName", "Tests with the same name are not allowed" )
{

}
*/

struct Boolable
{
    explicit Boolable( bool value ) : m_value( value ) {}

    operator Catch::SafeBool::type() const {
        return Catch::SafeBool::makeSafe( m_value );
    }

    bool m_value;
};

TEST_CASE( "./succeeding/SafeBool", "Objects that evaluated in boolean contexts can be checked")
{
    Boolable True( true );
    Boolable False( false );

    CHECK( True );
    CHECK( !False );
    CHECK_FALSE( False );
}

TEST_CASE( "Assertions then sections", "" )
{
    // This was causing a failure due to the way the console reporter was handling
    // the current section

    REQUIRE( Catch::isTrue( true ) );

    SECTION( "A section", "" )
    {
        REQUIRE( Catch::isTrue( true ) );

        SECTION( "Another section", "" )
        {
            REQUIRE( Catch::isTrue( true ) );
        }
        SECTION( "Another other section", "" )
        {
            REQUIRE( Catch::isTrue( true ) );
        }
    }
}
