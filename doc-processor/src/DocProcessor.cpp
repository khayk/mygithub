// doc-processor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DocProcessor.h"
#include "core/Converter.h"
#include <Poco/Util/HelpFormatter.h>
#include <boost/lexical_cast.hpp>

using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

DocProcessor::DocProcessor()
    : helpRequested_(false)
{
    setUnixOptions(true);
}

void DocProcessor::initialize( Application& self )
{
    // Initialize COM for this thread...
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    loadConfiguration();   // load default configuration files, if present
    Application::initialize(self);
}

void DocProcessor::uninitialize()
{
    Application::uninitialize();
    /// uninitialize COM
    CoUninitialize();
}

void DocProcessor::defineOptions( OptionSet& options )
{
    Application::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information on command line arguments")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback<DocProcessor>(this, &DocProcessor::handleHelp)));

}

void DocProcessor::handleHelp( const std::string& name, const std::string& value )
{
    helpRequested_ = true;
    displayHelp();
    stopOptionsProcessing();
}

void DocProcessor::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setUnixStyle(true);
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("[options] ...");
    helpFormatter.format(std::cout);
}

/// [i, j)
bool singleGroup(const string_t& a, int i, int j) {
    if (j > a.size())
        return false;

    for (int k = i; k < j; ++k)
        if (a[k] != a[i])
            return false;
    return true;
}

/// return an index where the group is finished
int extractGroup(const string_t& a, int st) {
    size_t n = 1;
    size_t end = st + n, lastEnd;
    
    bool skipCheck = false;
    while (true) {
        while ( skipCheck || singleGroup(a, st, end) ) {
            lastEnd = end;
            end += n;
            n *= 2;
            skipCheck = false;
        }

        if ( singleGroup(a, st, end - 1) )
            return end - 1;

        end -= (n / 2);
        n /= 4;
        if ( n == 0 )
            n = 1;
        skipCheck = true;
    }
}

int extractGroup1(const string_t& a, int st) {
    int en = st + 1;
    while (en < a.size() && a[en] == a[st]) {
        ++en;
    }
    return en;
}

int DocProcessor::main( const std::vector<std::string>& args )
{
/*    string_t a("111111111112222222223333333300100111299999977757100");
    //string_t a("122444455555555");
    for (int i = 0; i < 150000; ++i) {
        char ch = '0' + rand() % 10;
        string_t s(1 + rand() % 3, ch);
        a += s;
    }

    std::cout << "generated\n";
    int st = 0;
    //int en1 = extractGroup(49);

    while (st != a.size()) {
        int en  = extractGroup1(a, st);
        int en1 = extractGroup(a, st);

        if (en != en1) {
            std::cout << "fail" << std::endl;
            break;
        }
        //std::cout << a.substr(st, en - st) << std::endl;
        st = en;
    }
    return 0;*/

    if (helpRequested_)
        return Application::EXIT_OK;

    if (args.size() < 3) {
        displayHelp();
        return Application::EXIT_USAGE;
    }

    // perform the main task
    for (int i = 0; i < args.size(); ++i)
        logInfo(logger(), "args[" + boost::lexical_cast<string_t>(i) + "] = " + args[i]);

    Converter converter;
    converter.initialize(args[0], args[1], args[2]);
    converter.start();

    logger().setLevel(Poco::Message::PRIO_FATAL);
    return Application::EXIT_OK;
}

POCO_APP_MAIN(DocProcessor)