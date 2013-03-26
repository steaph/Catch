/*
 *  Created by Phil on 14/08/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_TEST_CASE_INFO_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_TEST_CASE_INFO_HPP_INCLUDED

#include "catch_tags.hpp"
#include "catch_test_case_info.h"
#include "catch_interfaces_testcase.h"
#include "catch_common.h"

namespace Catch {

    TestCase makeTestCase(  ITestCase* _testCase,
                            const std::string& _className,
                            const std::string& _name,
                            const std::string& _descOrTags,
                            const SourceLineInfo& _lineInfo )
    {
        std::string desc = _descOrTags;
        bool isHidden( startsWith( _name, "./" ) );
        std::set<std::string> tags;
        TagExtracter( tags ).parse( desc );
        if( tags.find( "hide" ) != tags.end() )
            isHidden = true;

        TestCaseInfo info( _name, _className, desc, tags, isHidden, _lineInfo );
        return TestCase( _testCase, info );
    }

    TestCaseInfo::TestCaseInfo( const std::string& _name,
                                const std::string& _className,
                                const std::string& _description,
                                const std::set<std::string>& _tags,
                                bool _isHidden,
                                const SourceLineInfo& _lineInfo )
    :   name( _name ),
        className( _className ),
        description( _description ),
        tags( _tags ),
        lineInfo( _lineInfo ),
        isHidden( _isHidden )
    {}

    TestCaseInfo::TestCaseInfo( const TestCaseInfo& other )
    :   name( other.name ),
        className( other.className ),
        description( other.description ),
        tags( other.tags ),
        lineInfo( other.lineInfo ),
        isHidden( other.isHidden )
    {}

    TestCase::TestCase( ITestCase* testCase, const TestCaseInfo& info ) : TestCaseInfo( info ), test( testCase ) {}

    TestCase::TestCase( const TestCase& other )
    :   TestCaseInfo( other ),
        test( other.test )
    {}

    TestCase TestCase::withName( const std::string& _newName ) const {
        TestCase other( *this );
        other.name = _newName;
        return other;
    }

    void TestCase::invoke() const {
        test->invoke();
    }

    bool TestCase::isHidden() const {
        return TestCaseInfo::isHidden;
    }

    bool TestCase::hasTag( const std::string& tag ) const {
        return tags.find( toLower( tag ) ) != tags.end();
    }
    bool TestCase::matchesTags( const std::string& tagPattern ) const {
        TagExpression exp;
        TagExpressionParser( exp ).parse( tagPattern );
        return exp.matches( tags );
    }
    const std::set<std::string>& TestCase::getTags() const {
        return tags;
    }

    void TestCase::swap( TestCase& other ) {
        test.swap( other.test );
        className.swap( other.className );
        name.swap( other.name );
        description.swap( other.description );
        std::swap( lineInfo, other.lineInfo );
    }

    bool TestCase::operator == ( const TestCase& other ) const {
        return  test.get() == other.test.get() &&
                name == other.name &&
                className == other.className;
    }

    bool TestCase::operator < ( const TestCase& other ) const {
        return name < other.name;
    }
    TestCase& TestCase::operator = ( const TestCase& other ) {
        TestCase temp( other );
        swap( temp );
        return *this;
    }

    const TestCaseInfo& TestCase::getTestCaseInfo() const
    {
        return *this;
    }

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_TEST_CASE_INFO_HPP_INCLUDED
