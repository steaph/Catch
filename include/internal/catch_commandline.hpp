/*
 *  Created by Phil on 02/11/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_COMMANDLINE_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_COMMANDLINE_HPP_INCLUDED

#include "catch_config.hpp"
#include "catch_common.h"

namespace Catch {

    class Command {
    public:
        Command(){}

        explicit Command( const std::string& name ) : m_name( name ) {
        }

        Command& operator += ( const std::string& arg ) {
            m_args.push_back( arg );
            return *this;
        }
        Command& operator += ( const Command& other ) {
            std::copy( other.m_args.begin(), other.m_args.end(), std::back_inserter( m_args ) );
            if( m_name.empty() )
                m_name = other.m_name;
            return *this;
        }
        Command operator + ( const Command& other ) {
            Command newCommand( *this );
            newCommand += other;
            return newCommand;
        }

        operator SafeBool::type() const {
            return SafeBool::makeSafe( !m_name.empty() || !m_args.empty() );
        }

        std::string name() const { return m_name; }
        std::string operator[]( std::size_t i ) const { return m_args[i]; }
        std::size_t argsCount() const { return m_args.size(); }

        CATCH_ATTRIBUTE_NORETURN
        void raiseError( const std::string& message ) const {
            std::ostringstream oss;
            if( m_name.empty() )
                oss << "Error while parsing " << m_name << ". " << message << ".";
            else
                oss << "Error while parsing arguments. " << message << ".";

            if( m_args.size() > 0 )
                oss << " Arguments were:";
            for( std::size_t i = 0; i < m_args.size(); ++i )
                oss << " " << m_args[i];
            throw std::domain_error( oss.str() );
        }

    private:

        std::string m_name;
        std::vector<std::string> m_args;
    };

    class CommandParser {
    public:
        CommandParser( int argc, char const * const * argv ) : m_argc( static_cast<std::size_t>( argc ) ), m_argv( argv ) {}

        Command find( const std::string& arg1,  const std::string& arg2, const std::string& arg3 ) const {
            return find( arg1 ) + find( arg2 ) + find( arg3 );
        }

        Command find( const std::string& shortArg, const std::string& longArg ) const {
            return find( shortArg ) + find( longArg );
        }
        Command find( const std::string& arg ) const {
            if( arg.empty() )
                return getArgs( "", 1 );
            else
                for( std::size_t i = 1; i < m_argc; ++i  )
                    if( m_argv[i] == arg )
                        return getArgs( m_argv[i], i+1 );
            return Command();
        }
        Command getDefaultArgs() const {
            return getArgs( "", 1 );
        }

    private:
        Command getArgs( const std::string& cmdName, std::size_t from ) const {
            Command command( cmdName );
            for( std::size_t i = from; i < m_argc && m_argv[i][0] != '-'; ++i  )
                command += m_argv[i];
            return command;
        }

        std::size_t m_argc;
        char const * const * m_argv;
    };

    class OptionParser : public SharedImpl<IShared> {
    public:
        OptionParser( int minArgs = 0, int maxArgs = 0 )
        : m_minArgs( minArgs ), m_maxArgs( maxArgs )
        {}

        virtual ~OptionParser() {}

        Command find( const CommandParser& parser ) const {
            Command cmd;
            for( std::vector<std::string>::const_iterator it = m_optionNames.begin();
                it != m_optionNames.end();
                ++it )
                cmd += parser.find( *it );
            return cmd;
        }

        void validateArgs( const Command& args ) const {
            if(  tooFewArgs( args ) || tooManyArgs( args ) ) {
                std::ostringstream oss;
                if( m_maxArgs == -1 )
                    oss <<"Expected at least " << pluralise( static_cast<std::size_t>( m_minArgs ), "argument" );
                else if( m_minArgs == m_maxArgs )
                    oss <<"Expected " << pluralise( static_cast<std::size_t>( m_minArgs ), "argument" );
                else
                    oss <<"Expected between " << m_minArgs << " and " << m_maxArgs << " argument";
                args.raiseError( oss.str() );
            }
        }

        void parseIntoConfig( const CommandParser& parser, ConfigData& config ) {
            if( Command cmd = find( parser ) ) {
                validateArgs( cmd );
                parseIntoConfig( cmd, config );
            }
        }

        virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) = 0;
        virtual std::string argsSynopsis() const = 0;
        virtual std::string optionSummary() const = 0;

        std::string optionNames() const {
            std::string names;
            for(    std::vector<std::string>::const_iterator it = m_optionNames.begin();
                    it != m_optionNames.end();
                    ++it ) {
                if( !it->empty() ) {
                    if( !names.empty() )
                        names += ", ";
                    names += *it;
                }
                else {
                    names = "[" + names;
                }
            }
            if( names[0] == '[' )
                names += "]";
            return names;
        }

    protected:

        bool tooFewArgs( const Command& args ) const {
            return args.argsCount() < static_cast<std::size_t>( m_minArgs );
        }
        bool tooManyArgs( const Command& args ) const {
            return m_maxArgs >= 0 && args.argsCount() > static_cast<std::size_t>( m_maxArgs );
        }
        std::vector<std::string> m_optionNames;
        int m_minArgs;
        int m_maxArgs;
    };

    namespace Options {

        class HelpOptionParser : public OptionParser {
        public:
            HelpOptionParser() {
                m_optionNames.push_back( "-?" );
                m_optionNames.push_back( "-h" );
                m_optionNames.push_back( "--help" );
            }
            virtual std::string argsSynopsis() const {
                return "";
            }
            virtual std::string optionSummary() const {
                return "Shows this usage summary";
            }

            virtual void parseIntoConfig( const Command&, ConfigData& ) {
                // Does not affect config
            }
        };

        class TestCaseOptionParser : public OptionParser {
        public:
            TestCaseOptionParser() : OptionParser( 1, -1 ) {
                m_optionNames.push_back( "-t" );
                m_optionNames.push_back( "--test" );
                m_optionNames.push_back( "" ); // default option
            }
            virtual std::string argsSynopsis() const {
                return "<testspec> [<testspec>...]";
            }
            virtual std::string optionSummary() const {
                return "Specify which test case or cases to run";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                std::string groupName;
                for( std::size_t i = 0; i < cmd.argsCount(); ++i ) {
                    if( i != 0 )
                        groupName += " ";
                    groupName += cmd[i];
                }
                TestCaseFilters filters( groupName );
                {for( std::size_t i = 0; i < cmd.argsCount(); ++i ) {
                    if( startsWith( cmd[i], "exclude:" ) )
                        filters.addFilter( TestCaseFilter( cmd[i].substr( 8 ), IfFilterMatches::ExcludeTests ) );
                    else if( startsWith( cmd[i], "~" ) )
                        filters.addFilter( TestCaseFilter( cmd[i].substr( 1 ), IfFilterMatches::ExcludeTests ) );
                    else
                        filters.addFilter( TestCaseFilter( cmd[i] ) );
                }}
                config.filters.push_back( filters );
            }
        };

        class ListOptionParser : public OptionParser {
        public:
            ListOptionParser() : OptionParser( 0, 2 ) {
                m_optionNames.push_back( "-l" );
                m_optionNames.push_back( "--list" );
            }
            virtual std::string argsSynopsis() const {
                return "[all | tests | reporters [xml]]";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                config.listSpec = List::TestNames;
                if( cmd.argsCount() >= 1 ) {
                    if( cmd[0] == "all" )
                        config.listSpec = List::All;
                    else if( cmd[0] == "tests" )
                        config.listSpec = List::Tests;
                    else if( cmd[0] == "reporters" )
                        config.listSpec = List::Reports;
                    else
                        cmd.raiseError( "Expected [tests] or [reporters]" );
                }
                if( cmd.argsCount() >= 2 ) {
                    if( cmd[1] == "xml" )
                        config.listSpec = static_cast<List::What>( config.listSpec | List::AsXml );
                    else if( cmd[1] == "text" )
                        config.listSpec = static_cast<List::What>( config.listSpec | List::AsText );
                    else
                        cmd.raiseError( "Expected [xml] or [text]" );
                }
            }
        };

        class ReporterOptionParser : public OptionParser {
        public:
            ReporterOptionParser() : OptionParser( 1, 1 ) {
                m_optionNames.push_back( "-r" );
                m_optionNames.push_back( "--reporter" );
            }
            virtual std::string argsSynopsis() const {
                return "<reporter name>";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                config.reporter = cmd[0];
            }
        };

        class OutputOptionParser : public OptionParser {
        public:
            OutputOptionParser() : OptionParser( 1, 1 ) {
                m_optionNames.push_back( "-o" );
                m_optionNames.push_back( "--out" );
            }
            virtual std::string argsSynopsis() const {
                return "<file name>|<%stream name>";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                if( cmd[0][0] == '%' )
                    config.stream = cmd[0].substr( 1 );
                else
                    config.outputFilename = cmd[0];
            }
        };

        class SuccesssOptionParser : public OptionParser {
        public:
            SuccesssOptionParser() {
                m_optionNames.push_back( "-s" );
                m_optionNames.push_back( "--success" );
            }
            virtual std::string argsSynopsis() const {
                return "";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command&, ConfigData& config ) {
                config.includeWhichResults = Include::SuccessfulResults;
            }
        };

        class DebugBreakOptionParser : public OptionParser {
        public:
            DebugBreakOptionParser() {
                m_optionNames.push_back( "-b" );
                m_optionNames.push_back( "--break" );
            }
            virtual std::string argsSynopsis() const {
                return "";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command&, ConfigData& config ) {
                config.shouldDebugBreak = true;
            }
        };

        class NameOptionParser : public OptionParser {
        public:
            NameOptionParser() : OptionParser( 1, 1 ) {
                m_optionNames.push_back( "-n" );
                m_optionNames.push_back( "--name" );
            }
            virtual std::string argsSynopsis() const {
                return "<name>";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                config.name = cmd[0];
            }
        };

        class AbortOptionParser : public OptionParser {
        public:
            AbortOptionParser() : OptionParser( 0, 1 ) {
                m_optionNames.push_back( "-a" );
                m_optionNames.push_back( "--abort" );
            }
            virtual std::string argsSynopsis() const {
                return "[#]";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                int threshold = 1;
                if( cmd.argsCount() == 1 ) {
                    std::stringstream ss;
                    ss << cmd[0];
                    ss >> threshold;
                    if( ss.fail() || threshold <= 0 )
                        cmd.raiseError( "threshold must be a number greater than zero" );
                }
                config.cutoff = threshold;
            }
        };

        class NoThrowOptionParser : public OptionParser {
        public:
            NoThrowOptionParser() {
                m_optionNames.push_back( "-nt" );
                m_optionNames.push_back( "--nothrow" );
            }
            virtual std::string argsSynopsis() const {
                return "";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command&, ConfigData& config ) {
                config.allowThrows = false;
            }
        };

        class WarningsOptionParser : public OptionParser {
        public:
            WarningsOptionParser() : OptionParser( 1, -1 ) {
                m_optionNames.push_back( "-w" );
                m_optionNames.push_back( "--warnings" );
            }
            virtual std::string argsSynopsis() const {
                return "<warning>";
            }
            virtual std::string optionSummary() const {
                return "!TBD";
            }

            virtual void parseIntoConfig( const Command& cmd, ConfigData& config ) {
                for( std::size_t i = 0; i < cmd.argsCount(); ++i ) {
                    if( cmd[i] == "NoAssertions" )
                        config.warnings = (ConfigData::WarnAbout::What)( config.warnings | ConfigData::WarnAbout::NoAssertions );
                    else
                        cmd.raiseError( "Unrecognised warning: " + cmd[i] );
                }
            }
        };
    }

    class AllOptions
    {
    public:
        typedef std::vector<Ptr<OptionParser> > Parsers;
        typedef Parsers::const_iterator const_iterator;
        typedef Parsers::const_iterator iterator;

        AllOptions() {
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
            add(Options::TestCaseOptionParser());   // Keep this one first

            add(Options::ListOptionParser());
            add(Options::ReporterOptionParser());
            add(Options::OutputOptionParser());
            add(Options::SuccesssOptionParser());
            add(Options::DebugBreakOptionParser());
            add(Options::NameOptionParser());
            add(Options::AbortOptionParser());
            add(Options::NoThrowOptionParser());
            add(Options::WarningsOptionParser());

            add(Options::HelpOptionParser());       // Keep this one last
#else
            add<Options::TestCaseOptionParser>();   // Keep this one first

            add<Options::ListOptionParser>();
            add<Options::ReporterOptionParser>();
            add<Options::OutputOptionParser>();
            add<Options::SuccesssOptionParser>();
            add<Options::DebugBreakOptionParser>();
            add<Options::NameOptionParser>();
            add<Options::AbortOptionParser>();
            add<Options::NoThrowOptionParser>();
            add<Options::WarningsOptionParser>();

            add<Options::HelpOptionParser>();       // Keep this one last
#endif
        }

        void parseIntoConfig( const CommandParser& parser, ConfigData& config ) {
            for( const_iterator it = m_parsers.begin(); it != m_parsers.end(); ++it )
                (*it)->parseIntoConfig( parser, config );
        }

        const_iterator begin() const {
            return m_parsers.begin();
        }
        const_iterator end() const {
            return m_parsers.end();
        }
    private:

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        void add( T & ) {
#else
        void add() {
#endif
            m_parsers.push_back( new T() );
        }
        Parsers m_parsers;

    };

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_COMMANDLINE_HPP_INCLUDED
