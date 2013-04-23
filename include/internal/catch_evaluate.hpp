/*
 *  Created by Phil on 04/03/2011.
 *  Copyright 2011 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4389) // '==' : signed/unsigned mismatch
#endif

namespace Catch {

namespace Detail {
    // required for VC6:
    class Approx;
}

namespace Internal {

    enum Operator {
        IsEqualTo,
        IsNotEqualTo,
        IsLessThan,
        IsGreaterThan,
        IsLessThanOrEqualTo,
        IsGreaterThanOrEqualTo
    };

    template<Operator Op> struct OperatorTraits             { static const char* getName(){ return "*error*"; } };
    template<> struct OperatorTraits<IsEqualTo>             { static const char* getName(){ return "=="; } };
    template<> struct OperatorTraits<IsNotEqualTo>          { static const char* getName(){ return "!="; } };
    template<> struct OperatorTraits<IsLessThan>            { static const char* getName(){ return "<"; } };
    template<> struct OperatorTraits<IsGreaterThan>         { static const char* getName(){ return ">"; } };
    template<> struct OperatorTraits<IsLessThanOrEqualTo>   { static const char* getName(){ return "<="; } };
    template<> struct OperatorTraits<IsGreaterThanOrEqualTo>{ static const char* getName(){ return ">="; } };
    template<typename T>
    inline T& opCast(T const& t) { return const_cast<T&>(t); }

// nullptr_t support based on pull request #154 from Konstantin Baumann
#ifdef CATCH_CONFIG_CPP11_NULLPTR
    inline std::nullptr_t opCast(std::nullptr_t) { return nullptr; }
#endif // CATCH_CONFIG_CPP11_NULLPTR


    // So the compare overloads can be operator agnostic we convey the operator as a template
    // enum, which is used to specialise an Evaluator for doing the comparison.
    template<Operator Op>
    struct Evaluator
    {
       template<typename T1, typename T2>
       static bool evaluate( T1 const& lhs, T2 const& rhs);
    };

    template<>
    struct Evaluator<IsEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) ==  opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsNotEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) != opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsLessThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) < opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsGreaterThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) > opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsGreaterThanOrEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) >= opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsLessThanOrEqualTo>
    {
        template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) <= opCast( rhs );
        }
    };

    template<Operator Op>
    struct Comparator
    {
#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6

        template<typename T1, typename T2>
        static bool compare( T1 const& lhs, T2 const& rhs ) {
            return Evaluator<Op>::evaluate( lhs, rhs );
        }
#else
class ::Catch::Detail::Approx;

#define INTERNAL_CATCH_DEFINE_COMPARE( LhsT, RhsT ) \
        static bool compare( LhsT const& lhs, RhsT const& rhs ) { \
            return Evaluator<Op>::evaluate( lhs, rhs ); \
        }

        INTERNAL_CATCH_DEFINE_COMPARE( int, int )
        INTERNAL_CATCH_DEFINE_COMPARE( int, long )
        INTERNAL_CATCH_DEFINE_COMPARE( int, float )
        INTERNAL_CATCH_DEFINE_COMPARE( int, double )
        INTERNAL_CATCH_DEFINE_COMPARE( int, ::Catch::Detail::Approx )

        INTERNAL_CATCH_DEFINE_COMPARE( long, int )
        INTERNAL_CATCH_DEFINE_COMPARE( long, long )
        INTERNAL_CATCH_DEFINE_COMPARE( long, float )
        INTERNAL_CATCH_DEFINE_COMPARE( long, double )
        INTERNAL_CATCH_DEFINE_COMPARE( long, ::Catch::Detail::Approx )

        INTERNAL_CATCH_DEFINE_COMPARE( float, int )
        INTERNAL_CATCH_DEFINE_COMPARE( float, long )
        INTERNAL_CATCH_DEFINE_COMPARE( float, float )
        INTERNAL_CATCH_DEFINE_COMPARE( float, double )
        INTERNAL_CATCH_DEFINE_COMPARE( float, ::Catch::Detail::Approx )

        INTERNAL_CATCH_DEFINE_COMPARE( double, int )
        INTERNAL_CATCH_DEFINE_COMPARE( double, long )
        INTERNAL_CATCH_DEFINE_COMPARE( double, float )
        INTERNAL_CATCH_DEFINE_COMPARE( double, double )
        INTERNAL_CATCH_DEFINE_COMPARE( double, ::Catch::Detail::Approx )

        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, int )
        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, long )
        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, float )
        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, double )

//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, int )
//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, long )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, float )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, double )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, ::Catch::Detail::Approx )

//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, int )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, long )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, float )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, double )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, ::Catch::Detail::Approx )

//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( int, unsigned int )
//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( int, unsigned long )

//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( long, unsigned int )
//      exclude: error C2668: 'compare' : ambiguous call to overloaded function
//        INTERNAL_CATCH_DEFINE_COMPARE( long, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, unsigned int )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned int, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, unsigned int )
        INTERNAL_CATCH_DEFINE_COMPARE( unsigned long, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( float, unsigned int )
        INTERNAL_CATCH_DEFINE_COMPARE( float, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( double, unsigned int )
        INTERNAL_CATCH_DEFINE_COMPARE( double, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, unsigned int )
        INTERNAL_CATCH_DEFINE_COMPARE( ::Catch::Detail::Approx, unsigned long )

        INTERNAL_CATCH_DEFINE_COMPARE( std::string, std::string )

#undef INTERNAL_CATCH_DEFINE_COMPARE
#endif
        // unsigned X to int
        static bool compare( unsigned int lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }
        static bool compare( unsigned long lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }
        static bool compare( unsigned char lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }

        // unsigned X to long
        static bool compare( unsigned int lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }
        static bool compare( unsigned long lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }
        static bool compare( unsigned char lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }

        // int to unsigned X
        static bool compare( int lhs, unsigned int rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }
        static bool compare( int lhs, unsigned long rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }
        static bool compare( int lhs, unsigned char rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }

        // long to unsigned X
        static bool compare( long lhs, unsigned int rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }
        static bool compare( long lhs, unsigned long rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }
        static bool compare( long lhs, unsigned char rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }

        // pointer to long (when comparing against NULL)
        template<typename T>
        static bool compare( long lhs, T const* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T const*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T const* lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T const*>( rhs ) );
        }

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
        template<typename T>
        static bool compare( long lhs, T* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T* lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
        }
#endif

        // pointer to int (when comparing against NULL)
        template<typename T>
        static bool compare( int lhs, T const* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T const*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T const* lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T const*>( rhs ) );
        }

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
        template<typename T>
        static bool compare( int lhs, T* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T* lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
        }
#endif

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        template<typename T>
        static bool compare( T* const & lhs, T* const & rhs ) {
            return Evaluator<Op>::evaluate( lhs, rhs );
        }

        template<typename T>
        static bool compare( const std::vector<T>& lhs, const std::vector<T>& rhs ) { \
            return Evaluator<Op>::evaluate( lhs, rhs ); \
        }
#endif
    };

#ifdef CATCH_CONFIG_CPP11_NULLPTR
    // pointer to nullptr_t (when comparing against nullptr)
    template<Operator Op, typename T> bool compare( std::nullptr_t, T* rhs ) {
        return Evaluator<T*, T*, Op>::evaluate( NULL, rhs );
    }
    template<Operator Op, typename T> bool compare( T* lhs, std::nullptr_t ) {
        return Evaluator<T*, T*, Op>::evaluate( lhs, NULL );
    }
#endif // CATCH_CONFIG_CPP11_NULLPTR

} // end of namespace Internal
} // end of namespace Catch

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
