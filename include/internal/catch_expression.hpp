/*
 *  Created by Phil on 11/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EXPRESSION_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EXPRESSION_HPP_INCLUDED

#include "catch_assertionresult_builder.h"
#include "catch_evaluate.hpp"

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#pragma warning( push )
#pragma warning( disable: 4800 )
#endif

namespace Catch {

template<typename T>
class Expression {
	void operator = ( const Expression& );

public:
    Expression( AssertionResultBuilder& result, T lhs )
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
// prevent  error C2354: initialization of reference member requires a temporary variable
    :   m_result( result ),
        m_lhs( lhs )
    { (void) result.setLhs( Catch::toString( lhs ) ); }
#else
    :   m_result( result.setLhs( Catch::toString( lhs ) ) ),
        m_lhs( lhs )
    {}
#endif
    template<typename RhsT>
    AssertionResultBuilder& operator == ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    template<typename RhsT>
    AssertionResultBuilder& operator != ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsNotEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    template<typename RhsT>
    AssertionResultBuilder& operator < ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsLessThan>(m_result).captureExpression( m_lhs, rhs );
    }

    template<typename RhsT>
    AssertionResultBuilder& operator > ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsGreaterThan>(m_result).captureExpression( m_lhs, rhs );
    }

    template<typename RhsT>
    AssertionResultBuilder& operator <= ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsLessThanOrEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    template<typename RhsT>
    AssertionResultBuilder& operator >= ( const RhsT& rhs ) {
        return Internal::Apply<Internal::IsGreaterThanOrEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    AssertionResultBuilder& operator == ( bool rhs ) {
        return Internal::Apply<Internal::IsEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    AssertionResultBuilder& operator != ( bool rhs ) {
        return Internal::Apply<Internal::IsNotEqualTo>(m_result).captureExpression( m_lhs, rhs );
    }

    operator AssertionResultBuilder& () {
        return m_result.setResultType( m_lhs ? ResultWas::Ok : ResultWas::ExpressionFailed );
    }

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator + ( const RhsT& );

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator - ( const RhsT& );

private:
    template<Internal::Operator Op, typename RhsT>
    AssertionResultBuilder& captureExpression( const RhsT& rhs ) {
        return m_result
            .setResultType( Internal::Comparator<Op>::compare( m_lhs, rhs ) ? ResultWas::Ok : ResultWas::ExpressionFailed )
            .setRhs( Catch::toString( rhs ) )
            .setOp( Internal::OperatorTraits<Op>::getName() );
    }

private:
    AssertionResultBuilder& m_result;
    T m_lhs;
};

} // end namespace Catch

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#pragma warning( pop )
#endif

#endif // TWOBLUECUBES_CATCH_EXPRESSION_HPP_INCLUDED
