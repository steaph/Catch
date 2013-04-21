/*
 *  Created by Martin on 21/4/2013.
 *  Copyright 2013 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_META_VC6_H_INCLUDED
#define TWOBLUECUBES_CATCH_META_VC6_H_INCLUDED

#include <vector>

namespace Catch {

template <int v>
struct IntToType { enum { value = v }; };

struct YesType { char c; };
struct NoType  { char c[2]; };

// is_pointer

namespace Detail {

struct isPointerHelper
{
   isPointerHelper( const volatile void* );
};

YesType isPointerTester( isPointerHelper );
NoType  isPointerTester( ... );

template <typename T>
struct isPointerImpl
{
   static T& makeT();
   enum { value = sizeof( YesType ) == sizeof( isPointerTester( makeT() ) ) };
};

} // namespace Detail

template < typename T >
struct isPointer
{
   enum { value = Detail::isPointerImpl<T>::value };
};

// is_vector

namespace Detail {

template<typename T>
YesType isVectorTester( const std::vector<T>& );

NoType isVectorTester( ... );

template <typename T>
struct isVectorImpl
{
   static T& makeT();
   enum { value = sizeof( YesType ) == sizeof( isVectorTester( makeT() ) ) };
};

} // namespace Detail

template <typename T>
struct isVector
{
   static T& makeT();
   enum { value = Detail::isVectorImpl<T>::value };
};

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_META_VC6_H_INCLUDED
