/*
 *  Created by Phil on 15/04/2013.
 *  Copyright 2013 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_COMPILER_CAPABILITIES_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_COMPILER_CAPABILITIES_HPP_INCLUDED

// Much of the following code is based on Boost (1.53)

////////////////////////////////////////////////////////////////////////////////
// Borland
#ifdef __BORLANDC__

#if (__BORLANDC__ > 0x582 )
//#define CATCH_SFINAE // Not confirmed
#endif

#endif // __BORLANDC__

////////////////////////////////////////////////////////////////////////////////
// EDG
#ifdef __EDG_VERSION__

#if (__EDG_VERSION__ > 238 )
//#define CATCH_SFINAE // Not confirmed
#endif

#endif // __EDG_VERSION__

////////////////////////////////////////////////////////////////////////////////
// Digital Mars
#ifdef __DMC__

#if (__DMC__ > 0x840 )
//#define CATCH_SFINAE // Not confirmed
#endif

#endif // __DMC__

////////////////////////////////////////////////////////////////////////////////
// GCC
#ifdef __GNUC__

#if __GNUC__ < 3

#if (__GNUC_MINOR__ >= 96 )
#define CATCH_SFINAE
#endif

#elif __GNUC__ >= 3

#define CATCH_SFINAE

#endif // __GNUC__ < 3


#endif // __GNUC__

////////////////////////////////////////////////////////////////////////////////
// Visual C++
#ifdef _MSC_VER

#if (_MSC_VER >= 1310 ) // (VC++ 7.0+)
//#define CATCH_SFINAE // Not confirmed
#endif

#endif // _MSC_VER


#endif // TWOBLUECUBES_CATCH_COMPILER_CAPABILITIES_HPP_INCLUDED

