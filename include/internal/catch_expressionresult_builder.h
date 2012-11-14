/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED
#define TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED

#include "catch_tostring.hpp"
#include "catch_assertionresult.h"
#include "catch_result_type.h"
#include "catch_evaluate.hpp"
#include "catch_common.h"

namespace Catch {

class ExpressionResultBuilder {
public:

    ExpressionResultBuilder( ResultWas::OfType resultType = ResultWas::Unknown );
    ExpressionResultBuilder( const ExpressionResultBuilder& other );
    ExpressionResultBuilder& operator=(const ExpressionResultBuilder& other );

    ExpressionResultBuilder& setResultType( ResultWas::OfType result );
    ExpressionResultBuilder& setResultType( bool result );
    ExpressionResultBuilder& setLhs( const std::string& lhs );
    ExpressionResultBuilder& setRhs( const std::string& rhs );
    ExpressionResultBuilder& setOp( const std::string& op );

    ExpressionResultBuilder& negate( bool shouldNegate );

    template<typename T>
    ExpressionResultBuilder& operator << ( const T& value ) {
        m_stream << value;
        return *this;
    }

    std::string reconstructExpression( const AssertionInfo& info ) const;

    AssertionResultData build( const AssertionInfo& info ) const;

private:
    AssertionResultData m_data;
public:
    struct ExprComponents {
        ExprComponents() : shouldNegate( false ) {}
        bool shouldNegate;
        std::string lhs, rhs, op;
    } m_exprComponents;
private:
    std::ostringstream m_stream;
};

namespace Internal {

    template<Operator Op>
    class Apply
    {
    public:
        Apply( ExpressionResultBuilder & result )
        : m_result( result ) {}

        template<typename T1, typename T2>
        ExpressionResultBuilder& captureExpression( const T1& lhs, const T2& rhs ) {
            m_result.setResultType( Comparator<Op>::compare( lhs, rhs ) ? ResultWas::Ok : ResultWas::ExpressionFailed );
            m_result.m_exprComponents.lhs = Catch::toString( lhs );
            m_result.m_exprComponents.rhs = Catch::toString( rhs );
            m_result.m_exprComponents.op = Internal::OperatorTraits<Op>::getName();
            return m_result;
        }

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        ExpressionResultBuilder& captureExpression( const T*& lhs, const int& rhs ) {
#else
        ExpressionResultBuilder& captureExpression( const T*  lhs, const int& rhs ) {
#endif
            return captureExpression( lhs, reinterpret_cast<const T*>( rhs ) );
        }

    private:
        ExpressionResultBuilder & m_result;
    };

} // end namespace Internal
} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED
