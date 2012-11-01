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

namespace Internal {
    template<Operator Op>
    class Apply;
}

struct STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison;

class AssertionResultBuilder {
public:

    AssertionResultBuilder();

    AssertionResultBuilder& setResultType( ResultWas::OfType result );
    AssertionResultBuilder& setCapturedExpression( const std::string& capturedExpression );
    AssertionResultBuilder& setIsFalse( bool isFalse );
    AssertionResultBuilder& setMessage( const std::string& message );
    AssertionResultBuilder& setLineInfo( const SourceLineInfo& lineInfo );
    AssertionResultBuilder& setLhs( const std::string& lhs );
    AssertionResultBuilder& setRhs( const std::string& rhs );
    AssertionResultBuilder& setOp( const std::string& op );
    AssertionResultBuilder& setMacroName( const std::string& macroName );

    std::string reconstructExpression() const;

    AssertionResult build() const;

    // Disable attempts to use || and && in expressions (without parantheses)
    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( const RhsT& );
    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( const RhsT& );

    bool getIsFalse() const {
        return m_isFalse;
    }

private:
    AssertionResultData m_data;
public:
    std::string m_lhs, m_rhs, m_op;
private:
    bool m_isFalse;
};

namespace Internal {

    template<Operator Op>
    class Apply
    {
    public:
        Apply( AssertionResultBuilder & result )
        : m_result( result ) {}

        template<typename T1, typename T2>
        AssertionResultBuilder& captureExpression( const T1& lhs, const T2& rhs ) {
            m_result.setResultType( Comparator<Op>::compare( lhs, rhs ) ? ResultWas::Ok : ResultWas::ExpressionFailed );
            m_result.m_lhs = Catch::toString( lhs );
            m_result.m_rhs = Catch::toString( rhs );
            m_result.m_op = Internal::OperatorTraits<Op>::getName();
            return m_result;
        }

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        AssertionResultBuilder& captureExpression( const T*& lhs, const int& rhs ) {
#else
        AssertionResultBuilder& captureExpression( const T*  lhs, const int& rhs ) {
#endif
            return captureExpression( lhs, reinterpret_cast<const T*>( rhs ) );
        }

    private:
        AssertionResultBuilder & m_result;
    };

} // end namespace Internal
} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED
