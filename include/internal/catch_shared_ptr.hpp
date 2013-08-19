/*
 *  Created by Martin on 19/08/2013.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_SHARED_PTR_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_SHARED_PTR_HPP_INCLUDED

#include "catch_common.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace Catch {

    // SharedPtr added for CATCH-VC6; see C++11 interface.

    template <typename T>
    class SharedPtr
    {
    public:
        typedef T element_type;

        explicit SharedPtr( T * const ptr )
        : m_ptr( ptr ), m_count( new long( 1 ) )
        {}

        SharedPtr( SharedPtr const & other )
        : m_ptr( other.m_ptr ), m_count( other.m_count )
        {
            addRef();
        }

        ~SharedPtr()
        {
            release();
        }

        SharedPtr& operator=( SharedPtr other )
        {
            other.swap( *this );
            return *this;
        }

        void reset( T * const ptr )
        {
            SharedPtr p( ptr );
            p.swap( *this );
        }

        void swap( SharedPtr & other )
        {
            std::swap( m_ptr, other.m_ptr );
            std::swap( m_count, other.m_ptr );
        }

        T * get() const { return m_ptr; }
        T * operator->() const { return m_ptr; }
        T & operator*() const { return *m_ptr; }

        long use_count() const { return m_count ? *m_count : 0; }

        operator SafeBool::type() const { return SafeBool::makeSafe( get() != NULL ); }

    private:
        void addRef()
        {
            if ( m_count )
                ++*m_count;
        }

        void release()
        {
            if( m_count && 0 == --*m_count )
            {
                delete m_ptr; m_ptr = 0;
                delete m_count; m_count = 0;
            }
        }

    private:
        T * m_ptr;
        long * m_count;
    };

} // end namespace Catch

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // TWOBLUECUBES_CATCH_SHARED_PTR_HPP_INCLUDED
