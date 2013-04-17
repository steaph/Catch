/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_TOSTRING_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_TOSTRING_HPP_INCLUDED

#include "catch_common.h"
#include "catch_sfinae.hpp"

#include <sstream>
#include <iomanip>
#include <limits>

#ifdef __OBJC__
#include "catch_objc_arc.hpp"
#endif

namespace Catch {

#ifdef CATCH_SFINAE

namespace Detail {

    template<typename T>
    class IsStreamInsertableHelper {
        template<int N> struct TrueIfSizeable : TrueType {};

        template<typename T2>
        static TrueIfSizeable<sizeof((*(std::ostream*)0) << *((T2 const*)0))> dummy(T2*);
        static FalseType dummy(...);

    public:
        typedef SizedIf<sizeof(dummy((T*)0))> type;
    };

    template<typename T>
    struct IsStreamInsertable : IsStreamInsertableHelper<T>::type {};

    template<typename T>
    void toStream( std::ostream& os, T const& value, typename IsStreamInsertable<T>::Enable* = 0 ) {
        os << value;
    }

    template<typename T>
    void toStream( std::ostream& os, T const&, typename IsStreamInsertable<T>::Disable* = 0 ) {
        os << "{?}";
    }

}

template<typename T>
struct StringMaker {
    static std::string convert( T const& value ) {
        std::ostringstream oss;
        Detail::toStream( oss, value );
        return oss.str();
    }
};

#else

namespace Detail {

    struct NonStreamable {
        template<typename T> NonStreamable( const T& ){}
    };

} // end namespace Detail

// If the type does not have its own << overload for ostream then
// this one will be used instead
inline std::ostream& operator << ( std::ostream& ss, Detail::NonStreamable ){
    return ss << "{?}";
}

template<typename T>
struct StringMaker {
    static std::string convert( T const& value ) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

#ifdef CATCH_COMPILER_IS_MSVC6

    static std::string convert( T const* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        std::ostringstream oss;
        oss << p;
        return oss.str();
    }

    static std::string convert( std::vector<T> const& v ) {
        std::ostringstream oss;
        oss << "{ ";
        for( std::size_t i = 0; i < v.size(); ++ i ) {
            oss << toString( v[i] );
            if( i < v.size() - 1 )
                oss << ", ";
        }
        oss << " }";
        return oss.str();
    }
#endif
};

#ifdef CATCH_COMPILER_IS_MSVC6

template<typename T>
struct StringMaker<T*> {
    static std::string convert( T const* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        std::ostringstream oss;
        oss << p;
        return oss.str();
    }

    static std::string convert( std::vector<T> const& v ) {
        std::ostringstream oss;
        oss << "{ ";
        for( std::size_t i = 0; i < v.size(); ++ i ) {
            oss << toString( v[i] );
            if( i < v.size() - 1 )
                oss << ", ";
        }
        oss << " }";
        return oss.str();
    }
};
#endif

#endif


namespace Detail {
    template<typename T>
    inline std::string makeString( const T& value ) {
        return StringMaker<T>::convert( value );
    }
} // end namespace Detail

/// \brief converts any type to a string
///
/// The default template forwards on to ostringstream - except when an
/// ostringstream overload does not exist - in which case it attempts to detect
/// that and writes {?}.
/// Overload (not specialise) this template for custom typs that you don't want
/// to provide an ostream overload for.
template<typename T>
std::string toString( const T& value ) {
    return StringMaker<T>::convert( value );
}

// Built in overloads

inline std::string toString( const std::string& value ) {
    return "\"" + value + "\"";
}

inline std::string toString( const std::wstring& value ) {
    std::ostringstream oss;
    oss << "\"";
    for(size_t i = 0; i < value.size(); ++i )
        oss << static_cast<char>( value[i] <= 0xff ? value[i] : '?');
    oss << "\"";
    return oss.str();
}

inline std::string toString( const char* const value ) {
    return value ? Catch::toString( std::string( value ) ) : std::string( "{null string}" );
}

inline std::string toString( char* const value ) {
    return Catch::toString( static_cast<const char*>( value ) );
}

inline std::string toString( int value ) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

inline std::string toString( unsigned long value ) {
    std::ostringstream oss;
    if( value > 8192 )
        oss << "0x" << std::hex << value;
    else
        oss << value;
    return oss.str();
}

inline std::string toString( unsigned int value ) {
    return toString( static_cast<unsigned long>( value ) );
}

inline std::string toString( const double value ) {
    std::ostringstream oss;
    oss << std::setprecision (std::numeric_limits<double>::digits10 + 1)
        << value;
    return oss.str();
}

inline std::string toString( bool value ) {
    return value ? "true" : "false";
}

inline std::string toString( char value ) {
    return value < ' '
        ? toString( static_cast<unsigned int>( value ) )
        : Detail::makeString( value );
}

inline std::string toString( signed char value ) {
    return toString( static_cast<char>( value ) );
}

inline std::string toString( unsigned char value ) {
    return toString( static_cast<char>( value ) );
}

#ifdef CATCH_CONFIG_CPP11_NULLPTR
inline std::string toString( std::nullptr_t ) {
    return "nullptr";
}
#endif

#ifdef __OBJC__
    inline std::string toString( NSString const * const& nsstring ) {
        if( !nsstring )
            return "nil";
        return std::string( "@\"" ) + [nsstring UTF8String] + "\"";
    }
    inline std::string toString( NSString * CATCH_ARC_STRONG const& nsstring ) {
        if( !nsstring )
            return "nil";
        return std::string( "@\"" ) + [nsstring UTF8String] + "\"";
    }
    inline std::string toString( NSObject* const& nsObject ) {
        return toString( [nsObject description] );
    }
#endif

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_TOSTRING_HPP_INCLUDED
