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

        explicit Command( std::string const& name ) : m_name( name ) {
        }

        Command& operator += ( std::string const& arg ) {
            m_args.push_back( arg );
            return *this;
        }
        Command& operator += ( Command const& other ) {
            std::copy( other.m_args.begin(), other.m_args.end(), std::back_inserter( m_args ) );
            if( m_name.empty() )
                m_name = other.m_name;
            return *this;
        }
        Command operator + ( Command const& other ) {
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

        void raiseError( std::string const& message ) const {
            std::ostringstream oss;
            if( m_name.empty() )
                oss << "Error while parsing " << m_name << ". " << message << ".";
            else
                oss << "Error while parsing arguments. " << message << ".";

            if( m_args.size() > 0 )
                oss << " Arguments were:";
            for( std::size_t i = 0; i < m_args.size(); ++i )
                oss << " " << m_args[i];
            if( isTrue( true ) )
                throw std::domain_error( oss.str() );
        }

    private:

        std::string m_name;
        std::vector<std::string> m_args;
    };

    class CommandParser {
    public:
        CommandParser( int argc, char const * const * argv ) : m_argc( static_cast<std::size_t>( argc ) ), m_argv( argv ) {}

        std::string exeName() const {
            std::string exeName = m_argv[0];
            std::string::size_type pos = exeName.find_last_of( "/\\" );
            if( pos != std::string::npos )
                exeName = exeName.substr( pos+1 );
            return exeName;
        }
        Command find( std::string const& arg1,  std::string const& arg2, std::string const& arg3 ) const {
            return find( arg1 ) + find( arg2 ) + find( arg3 );
        }

        Command find( std::string const& shortArg, std::string const& longArg ) const {
            return find( shortArg ) + find( longArg );
        }
        Command find( std::string const& arg ) const {
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
        Command getArgs( std::string const& cmdName, std::size_t from ) const {
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

        Command find( CommandParser const& parser ) const {
            Command cmd;
            for( std::vector<std::string>::const_iterator it = m_optionNames.begin();
                it != m_optionNames.end();
                ++it )
                cmd += parser.find( *it );
            return cmd;
        }

        void validateArgs( Command const& args ) const {
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

        void parseIntoConfig( CommandParser const& parser, ConfigData& config ) {
            if( Command cmd = find( parser ) ) {
                validateArgs( cmd );
                parseIntoConfig( cmd, config );
            }
        }

        virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) = 0;
        virtual std::string argsSynopsis() const = 0;
        virtual std::string optionSummary() const = 0;
        virtual std::string optionDescription() const { return ""; }

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

        bool tooFewArgs( Command const& args ) const {
            return args.argsCount() < static_cast<std::size_t>( m_minArgs );
        }
        bool tooManyArgs( Command const& args ) const {
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
                return "[<option for help on> ...]";
            }
            virtual std::string optionSummary() const {
                return "Shows this usage summary, or help on a specific option, or options, if supplied";
            }
            virtual std::string optionDescription() const {
                return "";
            }

            virtual void parseIntoConfig( Command const&, ConfigData& ) {
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
                return "Specifies which test case or cases to run";
            }

            // Lines are split at the nearest prior space char to the 80 char column.
            // Tab chars are removed from the output but their positions are used to align
            // subsequently wrapped lines
            virtual std::string optionDescription() const {
                return
                    "This option allows one ore more test specs to be supplied. Each spec either fully "
                    "specifies a test case or is a pattern containing wildcards to match a set of test "
                    "cases. If this option is not provided then all test cases, except those prefixed "
                    "by './' are run\n"
                    "\n"
                    "Specs must be enclosed in \"quotes\" if they contain spaces. If they do not "
                    "contain spaces the quotes are optional.\n"
                    "\n"
                    "Wildcards consist of the * character at the beginning, end, or both and can substitute for "
                    "any number of any characters (including none)\n"
                    "\n"
                    "If spec is prefixed with exclude: or the ~ character then the pattern matches an exclusion. "
                    "This means that tests matching the pattern are excluded from the set - even if a prior "
                    "inclusion spec included them. Subsequent inclusion specs will take precedence, however. "
                    "Inclusions and exclusions are evaluated in left-to-right order.\n"
                    "\n"
                    "Examples:\n"
                    "\n"
                    "    -t thisTestOnly        \tMatches the test case called, 'thisTestOnly'\n"
                    "    -t \"this test only\"    \tMatches the test case called, 'this test only'\n"
                    "    -t these/*             \tMatches all cases starting with 'these/'\n"
                    "    -t exclude:notThis     \tMatches all tests except, 'notThis'\n"
                    "    -t ~notThis            \tMatches all tests except, 'notThis'\n"
                    "    -t ~*private*          \tMatches all tests except those that contain 'private'\n"
                    "    -t a/* ~a/b/* a/b/c    \tMatches all tests that start with 'a/', except those "
                                                 "that start with 'a/b/', except 'a/b/c', which is included";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                for( std::size_t i = 0; i < cmd.argsCount(); ++i )
                    config.testsOrTags.push_back( cmd[i] );
            }
        };

        class TagOptionParser : public OptionParser {
        public:
            TagOptionParser() : OptionParser( 1, -1 ) {
                m_optionNames.push_back( "-g" );
                m_optionNames.push_back( "--tag" );
            }
            virtual std::string argsSynopsis() const {
                return "<tagspec> [,<tagspec>...]";
            }
            virtual std::string optionSummary() const {
                return "Matches test cases against tags or tag patterns";
            }

            // Lines are split at the nearest prior space char to the 80 char column.
            // Tab chars are removed from the output but their positions are used to align
            // subsequently wrapped lines
            virtual std::string optionDescription() const {
                return
                "This option allows one or more tags or tag patterns to be specified.\n"
                "Each tag is enclosed in square brackets. A series of tags form an AND expression "
                "wheras a comma seperated sequence forms an OR expression. e.g.:\n\n"
                "    -g [one][two],[three]\n\n"
                "This matches all tests tagged [one] and [two], as well as all tests tagged [three].\n\n"
                "Tags can be negated with the ~ character. This removes matching tests from the set. e.g.:\n\n"
                "    -g [one]~[two]\n\n"
                "matches all tests tagged [one], except those also tagged [two]";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                for( std::size_t i = 0; i < cmd.argsCount(); ++i )
                    config.testsOrTags.push_back( cmd[i] );
            }
        };


        class ListOptionParser : public OptionParser {
        public:
            ListOptionParser() : OptionParser( 0, 1 ) {
                m_optionNames.push_back( "-l" );
                m_optionNames.push_back( "--list" );
            }
            virtual std::string argsSynopsis() const {
//                return "[all | tests | reporters | tags [xml]]";
                return "[all | tests | reporters | tags]";
            }
            virtual std::string optionSummary() const {
                return "Lists available tests or reporters";
            }

            virtual std::string optionDescription() const {
                return
                    "With no arguments this option will list all registered tests - one per line.\n"
//                    "Supplying the xml argument formats the list as an xml document (which may be useful for "
//                    "consumption by other tools).\n"
                    "Supplying the tests or reporters lists tests or reporters respectively - with descriptions.\n"
                    "\n"
                    "Examples:\n"
                    "\n"
                    "    -l\n"
                    "    -l tests\n"
                    "    -l tags\n"
                    "    -l reporters xml\n"
                    ;//"    -l xml";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                if( cmd.argsCount() >= 1 ) {
                    if( cmd[0] == "all" ) {
                        config.listTests = true;
                        config.listTags = true;
                        config.listReporters = true;
                    }
                    else if( cmd[0] == "tests" )
                        config.listTests = true;
                    else if( cmd[0] == "tags" )
                        config.listTags = true;
                    else if( cmd[0] == "reporters" )
                        config.listReporters = true;
                    else
                        cmd.raiseError( "Expected tests, reporters or tags" );
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
                return "Specifies type of reporter";
            }

            virtual std::string optionDescription() const {
                return
                    "A reporter is an object that formats and structures the output of running "
                    "tests, and potentially summarises the results. By default the console reporter "
                    "is used which writes IDE friendly results. CATCH comes bundled with some "
                    "alternative reporters, but more can be added in client code.\n"
                    "\n"
                    "The bundled reporters are:\n"
                    "    -r console\n"
                    "    -r xml\n"
                    "    -r junit\n"
                    "\n"
                    "The JUnit reporter is an xml format that follows the structure of the JUnit "
                    "XML Report ANT task, as consumed by a number of third-party tools, "
                    "including Continuous Integration servers such as Jenkins.\n"
                    "If not otherwise needed, the standard XML reporter is preferred as this is "
                    "a streaming reporter, whereas the Junit reporter needs to hold all its "
                    "results until the end so it can write the overall results into attributes "
                    "of the root node.";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                config.reporterName = cmd[0];
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
                return "Sends output to a file or stream";
            }
            virtual std::string optionDescription() const {
                return
                    "Use this option to send all output to a file or a stream. By default output is "
                    "sent to stdout (note that uses of stdout and stderr from within test cases are "
                    "redirected and included in the report - so even stderr will effectively end up "
                    "on stdout). If the name begins with % it is interpreted as a stream. "
                    "Otherwise it is treated as a filename.\n"
                    "\n"
                    "Examples are:\n"
                    "\n"
                    "    -o filename.txt\n"
                    "    -o \"long filename.txt\"\n"
                    "    -o %stdout\n"
                    "    -o %stderr\n"
                    "    -o %debug    \t(The IDE's debug output window - currently only Windows' "
                                        "OutputDebugString is supported).";
            }
            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                config.outputFilename = cmd[0];
            }
        };

        class SuccessOptionParser : public OptionParser {
        public:
            SuccessOptionParser() {
                m_optionNames.push_back( "-s" );
                m_optionNames.push_back( "--success" );
            }
            virtual std::string argsSynopsis() const {
                return "";
            }
            virtual std::string optionSummary() const {
                return "Shows results for successful tests";
            }
            virtual std::string optionDescription() const {
                return
                    "Usually you only want to see reporting for failed tests. Sometimes it's useful "
                    "to see all the output (especially when you don't trust that that test you just "
                    "added worked first time!). To see successful, as well as failing, test results "
                    "just pass this option.";
            }
            virtual void parseIntoConfig( Command const&, ConfigData& config ) {
                config.showSuccessfulTests = true;
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
                return "Breaks into the debugger on failure";
            }
            virtual std::string optionDescription() const {
                return
                    "In some IDEs (currently XCode and Visual Studio) it is possible for CATCH to "
                    "break into the debugger on a test failure. This can be very helpful during "
                    "debug sessions - especially when there is more than one path through a "
                    "particular test. In addition to the command line option, ensure you have "
                    "built your code with the DEBUG preprocessor symbol";
            }

            virtual void parseIntoConfig( Command const&, ConfigData& config ) {
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
                return "Names a test run";
            }
            virtual std::string optionDescription() const {
                return
                    "If a name is supplied it will be used by the reporter to provide an overall "
                    "name for the test run. This can be useful if you are sending to a file, for "
                    "example, and need to distinguish different test runs - either from different "
                    "Catch executables or runs of the same executable with different options.\n"
                    "\n"
                    "Examples:\n"
                    "\n"
                    "    -n testRun\n"
                    "    -n \"tests of the widget component\"";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
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
                return "Aborts after a certain number of failures";
            }
            virtual std::string optionDescription() const {
                return
                    "If a REQUIRE assertion fails the test case aborts, but subsequent test cases "
                    "are still run. If a CHECK assertion fails even the current test case is not "
                    "aborted.\n"
                    "\n"
                    "Sometimes this results in a flood of failure messages and you'd rather just "
                    "see the first few. Specifying -a or --abort on its own will abort the whole "
                    "test run on the first failed assertion of any kind. Following it with a "
                    "number causes it to abort after that number of assertion failures.";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
                int threshold = 1;
                if( cmd.argsCount() == 1 ) {
                    std::stringstream ss;
                    ss << cmd[0];
                    ss >> threshold;
                    if( ss.fail() || threshold <= 0 )
                        cmd.raiseError( "threshold must be a number greater than zero" );
                }
                config.abortAfter = threshold;
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
                return "Elides assertions expected to throw";
            }
            virtual std::string optionDescription() const {
                return
                    "Skips all assertions that test that an exception is thrown, "
                    "e.g. REQUIRE_THROWS.\n"
                    "\n"
                    "These can be a nuisance in certain debugging environments that may break when "
                    "exceptions are thrown (while this is usually optional for handled exceptions, "
                    "it can be useful to have enabled if you are trying to track down something "
                    "unexpected).\n"
                    "\n"
                    "When running with this option the throw checking assertions are skipped so "
                    "as not to contribute additional noise.";
            }

            virtual void parseIntoConfig( Command const&, ConfigData& config ) {
                config.noThrow = true;
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
                return "Enable warnings";
            }
            virtual std::string optionDescription() const {
                return
                    "Enables the named warnings. If the warnings are violated the test case is "
                    "failed.\n"
                    "\n"
                    "At present only one warning has been provided: NoAssertions. If this warning "
                    "is enabled then any test case that completes without an assertions (CHECK, "
                    "REQUIRE etc) being encountered violates the warning.\n"
                    "\n"
                    "e.g.:\n"
                    "\n"
                    "    -w NoAssertions";
            }

            virtual void parseIntoConfig( Command const& cmd, ConfigData& config ) {
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

            add(Options::TagOptionParser());
            add(Options::ListOptionParser());
            add(Options::ReporterOptionParser());
            add(Options::OutputOptionParser());
            add(Options::SuccessOptionParser());
            add(Options::DebugBreakOptionParser());
            add(Options::NameOptionParser());
            add(Options::AbortOptionParser());
            add(Options::NoThrowOptionParser());
            add(Options::WarningsOptionParser());

            add(Options::HelpOptionParser());       // Keep this one last
#else
            add<Options::TestCaseOptionParser>();   // Keep this one first

            add<Options::TagOptionParser>();
            add<Options::ListOptionParser>();
            add<Options::ReporterOptionParser>();
            add<Options::OutputOptionParser>();
            add<Options::SuccessOptionParser>();
            add<Options::DebugBreakOptionParser>();
            add<Options::NameOptionParser>();
            add<Options::AbortOptionParser>();
            add<Options::NoThrowOptionParser>();
            add<Options::WarningsOptionParser>();

            add<Options::HelpOptionParser>();       // Keep this one last
#endif
        }

        void parseIntoConfig( CommandParser const& parser, ConfigData& config ) {
            config.name = parser.exeName();
            if( endsWith( config.name, ".exe" ) )
               config.name = config.name.substr( 0, config.name.size()-4 );
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
