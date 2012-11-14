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
    inline void setResultIfBoolean( AssertionResultBuilder&, const T& )
{
}
    inline void setResultIfBoolean( AssertionResultBuilder& result, bool value ) {
        result.setResultType( value );
    }

template<typename T>
class ExpressionLhs {
	void operator = ( const ExpressionLhs& );

public:
    ExpressionLhs( T lhs ) : m_lhs( lhs ) {
        setResultIfBoolean( m_result.setLhs( Catch::toString( lhs ) ), lhs );
    }

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

    AssertionResultBuilder negate( bool shouldNegate ) {
        return m_result.negate( shouldNegate );
    }

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator + ( const RhsT& );

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator - ( const RhsT& );

private:
    AssertionResultBuilder m_result;
    T m_lhs;
};

} // end namespace Catch

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#pragma warning( pop )
#endif

#endif // TWOBLUECUBES_CATCH_EXPRESSION_HPP_INCLUDED
