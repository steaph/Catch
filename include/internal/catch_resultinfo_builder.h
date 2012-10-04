/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_RESULTINFO_BUILDER_H_INCLUDED
#define TWOBLUECUBES_CATCH_RESULTINFO_BUILDER_H_INCLUDED

#include "catch_tostring.hpp"
#include "catch_resultinfo.h"
#include "catch_result_type.h"
#include "catch_evaluate.hpp"
#include "catch_common.h"

namespace Catch {

namespace Internal {
    template<Operator Op>
    class Apply;
}

struct STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison;

class ResultInfoBuilder : public ResultInfo {
public:

    ResultInfoBuilder();

    ResultInfoBuilder& setResultType( ResultWas::OfType result );
    ResultInfoBuilder& setMessage( const std::string& message );
    ResultInfoBuilder& setLineInfo( const SourceLineInfo& lineInfo );
    ResultInfoBuilder& setLhs( const std::string& lhs );
    ResultInfoBuilder& setRhs( const std::string& rhs );
    ResultInfoBuilder& setOp( const std::string& op );
    ResultInfoBuilder& setMacroName( const std::string& macroName );

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( const RhsT& );

    template<typename RhsT>
    STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( const RhsT& );

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
public:
#else
private:
#endif
    ResultInfoBuilder(  const char* expr,
                      bool isNot,
                      const SourceLineInfo& lineInfo,
                      const char* macroName );

#ifndef INTERNAL_CATCH_COMPILER_IS_MSVC6
private:

    friend class ExpressionBuilder;
    template<typename T> friend class Expression;

    template<typename T> friend class PtrExpression;
    template<Internal::Operator Op> friend class Internal::Apply;
#endif

    ResultInfoBuilder& captureBoolExpression( bool result );
};

namespace Internal {

    template<Operator Op>
    class Apply
    {
    public:
        Apply( ResultInfoBuilder & result )
        : m_result( result ) {}

        template<typename T1, typename T2>
        ResultInfoBuilder& captureExpression( const T1& lhs, const T2& rhs ) {
            m_result.setResultType( Comparator<Op>::compare( lhs, rhs ) ? ResultWas::Ok : ResultWas::ExpressionFailed );
            m_result.m_lhs = Catch::toString( lhs );
            m_result.m_rhs = Catch::toString( rhs );
            m_result.m_op = Internal::OperatorTraits<Op>::getName();
            return m_result;
        }

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        ResultInfoBuilder& captureExpression( const T*& lhs, const int& rhs ) {
#else
        ResultInfoBuilder& captureExpression( const T*  lhs, const int& rhs ) {
#endif
            return captureExpression( lhs, reinterpret_cast<const T*>( rhs ) );
        }

    private:
        ResultInfoBuilder & m_result;
    };

} // end namespace Internal
} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_RESULTINFO_BUILDER_H_INCLUDED
