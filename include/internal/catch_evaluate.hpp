/*
 *  Created by Phil on 04/03/2011.
 *  Copyright 2011 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED

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
    
    // So the compare overloads can be operator agnostic we convey the operator as a template
    // enum, which is used to specialise an Evaluator for doing the comparison.
    template<Operator Op>
    struct Evaluator
    {
       template<typename T1, typename T2>
       static bool evaluate( const T1& lhs, const T2& rhs);
    };
    
    template<>
    struct Evaluator<IsEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs == rhs;
#else
            return const_cast<T1&>( lhs ) == const_cast<T2&>( rhs );
#endif
        }
    };
    template<>
    struct Evaluator<IsNotEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs != rhs;
#else
            return const_cast<T1&>( lhs ) != const_cast<T2&>( rhs );
#endif
        }
    };
    template<>
    struct Evaluator<IsLessThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs < rhs;
#else
            return const_cast<T1&>( lhs ) < const_cast<T2&>( rhs );
#endif
        }
    };
    template<>
    struct Evaluator<IsGreaterThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs > rhs;
#else
            return const_cast<T1&>( lhs ) > const_cast<T2&>( rhs );
#endif
        }
    };
    template<>
    struct Evaluator<IsGreaterThanOrEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs >= rhs;
#else
            return const_cast<T1&>( lhs ) >= const_cast<T2&>( rhs );
#endif
        }
    };
    template<>
    struct Evaluator<IsLessThanOrEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( const T1& lhs, const T2& rhs) {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            return lhs <= rhs;
#else
            return const_cast<T1&>( lhs ) <= const_cast<T2&>( rhs );
#endif
        }
    };

    template<Operator Op, typename T1, typename T2>
    bool applyEvaluator( const T1& lhs, const T2& rhs ) {
        return Evaluator<Op>::evaluate( lhs, rhs );
    }

    // "base" overload
    template<Operator Op, typename T1, typename T2>
    bool compare( const T1& lhs, const T2& rhs ) {
        return Evaluator<Op>::evaluate( lhs, rhs );
    }

    // unsigned X to int
    template<Operator Op> bool compare( unsigned int lhs, int rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned long lhs, int rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned char lhs, int rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
    }
    
    // unsigned X to long
    template<Operator Op> bool compare( unsigned int lhs, long rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned long lhs, long rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
    }
    template<Operator Op> bool compare( unsigned char lhs, long rhs ) {
        return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
    }
    
    // int to unsigned X
    template<Operator Op> bool compare( int lhs, unsigned int rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
    }
    template<Operator Op> bool compare( int lhs, unsigned long rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
    }
    template<Operator Op> bool compare( int lhs, unsigned char rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
    }
    
    // long to unsigned X
    template<Operator Op> bool compare( long lhs, unsigned int rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
    }
    template<Operator Op> bool compare( long lhs, unsigned long rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
    }
    template<Operator Op> bool compare( long lhs, unsigned char rhs ) {
        return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
    }

    // pointer to long (when comparing against NULL)
    template<Operator Op, typename T>
    bool compare( long lhs, const T* rhs ) {
        return Evaluator<Op>::evaluate( reinterpret_cast<const T*>( lhs ), rhs );
    }
    
    template<Operator Op, typename T>
    bool compare( long lhs, T* rhs ) {
        return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
    }

    template<Operator Op, typename T>
    bool compare( const T* lhs, long rhs ) {
        return Evaluator<Op>::evaluate( lhs, reinterpret_cast<const T*>( rhs ) );
    }
    
    template<Operator Op, typename T>
    bool compare( T* lhs, long rhs ) {
        return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
    }
    
    // pointer to int (when comparing against NULL)
    template<Operator Op, typename T>
    bool compare( int lhs, const T* rhs ) {
        return Evaluator<Op>::evaluate( reinterpret_cast<const T*>( lhs ), rhs );
    }
    
    template<Operator Op, typename T>
    bool compare( int lhs, T* rhs ) {
        return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
    }

    template<Operator Op, typename T>
    bool compare( const T* lhs, int rhs ) {
        return Evaluator<Op>::evaluate( lhs, reinterpret_cast<const T*>( rhs ) );
    }
    
    template<Operator Op, typename T>
    bool compare( T* lhs, int rhs ) {
        return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
    }

} // end of namespace Internal
} // end of namespace Catch

#endif // TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
