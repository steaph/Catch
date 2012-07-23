/*
 *  Created by Phil on 29/10/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_COMMON_H_INCLUDED
#define TWOBLUECUBES_CATCH_COMMON_H_INCLUDED

#define INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line ) name##line
#define INTERNAL_CATCH_UNIQUE_NAME_LINE( name, line ) INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line )
#define INTERNAL_CATCH_UNIQUE_NAME( name ) INTERNAL_CATCH_UNIQUE_NAME_LINE( name, __LINE__ )

#define INTERNAL_CATCH_STRINGIFY2( expr ) #expr
#define INTERNAL_CATCH_STRINGIFY( expr ) INTERNAL_CATCH_STRINGIFY2( expr )

#ifdef _MSC_VER
#define INTERNAL_CATCH_COMPILER_IS_MSVC
#if ( _MSC_VER >= 1200 ) && ( _MSC_VER < 1300 )
#define INTERNAL_CATCH_COMPILER_IS_MSVC6
#endif
#endif

#ifdef __GNUC__
#define ATTRIBUTE_NORETURN __attribute__ ((noreturn))
#else
#define ATTRIBUTE_NORETURN
#endif

#include <sstream>
#include <stdexcept>
#include <algorithm>

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
namespace std { using ::size_t; }
#endif

namespace Catch {

	class NonCopyable {
		NonCopyable( const NonCopyable& );
		void operator = ( const NonCopyable& );
	protected:
		NonCopyable() {}
		virtual ~NonCopyable() {}
	};
    
    class SafeBool {
    public:
        typedef void (SafeBool::*type)() const;

        static type makeSafe( bool value ) {
            return value ? &SafeBool::trueValue : 0;
        }
    private:
        void trueValue() const {}
    };
  
    template<typename ContainerT>
    inline void deleteAll( ContainerT& container ) {
        typename ContainerT::const_iterator it = container.begin();
        typename ContainerT::const_iterator itEnd = container.end();
        for(; it != itEnd; ++it )
        {
            delete *it;
        }
    }
    template<typename AssociativeContainerT>
    inline void deleteAllValues( AssociativeContainerT& container ) {
        typename AssociativeContainerT::const_iterator it = container.begin();
        typename AssociativeContainerT::const_iterator itEnd = container.end();
        for(; it != itEnd; ++it )
        {
            delete it->second;
        }
    }
    
    template<typename ContainerT, typename Function>
    inline void forEach( ContainerT& container, Function function ) {
        std::for_each( container.begin(), container.end(), function );
    }
    
    template<typename ContainerT, typename Function>
    inline void forEach( const ContainerT& container, Function function ) {
        std::for_each( container.begin(), container.end(), function );
    }
    
    struct SourceLineInfo {
    
        SourceLineInfo() : line( 0 ){}
        SourceLineInfo( const std::string& _file, std::size_t _line )
        :   file( _file ),
            line( _line )
        {}
        SourceLineInfo( const std::string& _function, const std::string& _file, std::size_t _line )
        :   function( _function ),
            file( _file ),
            line( _line )
        {}
        SourceLineInfo( const SourceLineInfo& other )
        :   file( other.file ),
            line( other.line )
        {}
        void swap( SourceLineInfo& other ){
            file.swap( other.file );
            std::swap( line, other.line );
        }
        
        std::string function;
        std::string file;
        std::size_t line;        
    };
    
    inline std::ostream& operator << ( std::ostream& os, const SourceLineInfo& info ) {
#ifndef __GNUG__
        os << info.file << "(" << info.line << "): ";
#else                
        os << info.file << ":" << info.line << ": ";            
#endif            
        return os;
    }
    
    ATTRIBUTE_NORETURN
    inline void throwLogicError( const std::string& message, const std::string& file, std::size_t line ) {
        std::ostringstream oss;
        oss << "Internal Catch error: '" << message << "' at: " << SourceLineInfo( file, line );
        throw std::logic_error( oss.str() );
    }
}

#define CATCH_INTERNAL_ERROR( msg ) throwLogicError( msg, __FILE__, __LINE__ );

//#ifdef __FUNCTION__
//#define CATCH_INTERNAL_LINEINFO ::Catch::SourceLineInfo( __FUNCTION__, __FILE__, __LINE__ )
//#else
#define CATCH_INTERNAL_LINEINFO ::Catch::SourceLineInfo( __FILE__, __LINE__ )
//#endif

#endif // TWOBLUECUBES_CATCH_COMMON_H_INCLUDED

