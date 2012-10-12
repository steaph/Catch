/*
 *  Created by Phil on 28/10/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_RESULTINFO_H_INCLUDED
#define TWOBLUECUBES_CATCH_RESULTINFO_H_INCLUDED

#include <string>
#include "catch_result_type.h"

namespace Catch {

    struct ResultData
    {
        ResultData() : resultType( ResultWas::Unknown ) {}

        std::string macroName;
        SourceLineInfo lineInfo;
        std::string capturedExpression;
        std::string reconstructedExpression;
        std::string message;
        ResultWas::OfType resultType;
    };

    class ResultInfo {
    public:
        ResultInfo();
        ResultInfo( const ResultData& data );
        ~ResultInfo();

        bool ok() const;
        ResultWas::OfType getResultType() const;
        bool hasExpression() const;
        bool hasMessage() const;
        std::string getExpression() const;
        bool hasExpandedExpression() const;
        std::string getExpandedExpression() const;
        std::string getMessage() const;
        std::string getFilename() const;
        std::size_t getLine() const;
        std::string getTestMacroName() const;

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
    public:
#else
    protected:
#endif
        ResultData m_data;
    };

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_RESULTINFO_H_INCLUDED
