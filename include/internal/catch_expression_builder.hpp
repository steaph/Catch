/*
 *  Created by Phil on 11/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EXPRESSION_BUILDER_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EXPRESSION_BUILDER_HPP_INCLUDED

#include "catch_expression.hpp"
#include "catch_assertionresult_builder.h"

namespace Catch {

class ExpressionBuilder {
public:

    template<typename T>
    Expression<const T&> operator->* ( const T & operand ) {
        return Expression<const T&>( operand );
    }

    Expression<bool> operator->* ( bool value ) {
        return Expression<bool>( value );
    }
};

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_EXPRESSION_BUILDER_HPP_INCLUDED
