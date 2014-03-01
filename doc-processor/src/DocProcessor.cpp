// doc-processor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DocProcessor.h"
#include "core/Converter.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/File.h>
#include "utils/Common.h"

#include <boost/lexical_cast.hpp>
#include "mapping/ASCII_Chars_Unicode.h"

using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

class ChangeLoggerLevel {
public:
    ChangeLoggerLevel(DocProcessor* dp) : dp_(dp) {
       logInfo(dp_->logger(), makeCentralizedNote("APPLICATION STARTED", '-', 80));
    }
    ~ChangeLoggerLevel() {
        logInfo(dp_->logger(), makeCentralizedNote("APPLICATION ENDED", '-', 80));
        if (dp_)
            dp_->logger().setLevel(Poco::Message::PRIO_WARNING);
    }

private:
    DocProcessor* dp_;
};

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

    Poco::Path logFile(config().getString("logging.channels.fc.path", ""));
    Poco::File logPath(logFile.parent().makeDirectory());
    if ( !logPath.exists() )
        logPath.createDirectories();
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

    options.addOption(
        Option("input", "i", "bind option value to input.folder")
        .required(true)
        .repeatable(false)
        .argument("value")
        .binding("input.folder"));

    options.addOption(
        Option("output", "o", "bind option value to output.folder")
        .required(true)
        .repeatable(false)
        .argument("value")
        .binding("output.folder"));

    options.addOption(
        Option("mapping", "m", "bind option value to mapping.folder")
        .required(true)
        .repeatable(false)
        .argument("value")
        .binding("mapping.folder"));

    options.addOption(
        Option("working-mode", "w", "bind option value to working.mode")
        .required(true)
        .repeatable(false)
        .argument("value")
        .binding("working.mode"));
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


int DocProcessor::main( const std::vector<std::string>& args )
{
    ChangeLoggerLevel cll(this);
    wstring_t appVersion;
    getFileVersion(toUtf16(config().getString("application.path")), appVersion);
    logInfo(logger(), "Application version: " + toUtf8(appVersion));

//     wstring_t tt(L"Time New Roman");
//     string_t t;
//     int j = 0;
//     for (int i = 0; i < 5000000; ++i) {
//         t = toUtf8(tt);
//         if (t.size() > 0)
//             ++j;
//     }
// 
//     std::cout << j << std::endl;
//     return 0;
//    AsciiToUniMapping::exportArmenianASCII_Unicode("../config/mapping/armenian/char-mapping-default.txt");
//     AsciiToUniMapping::exportRussianASCII_Unicode("../config/mapping/russian/char-mapping-default.txt");
//    return Application::EXIT_OK;

/*    HDC hdc = CreateCompatibleDC(NULL);
    HFONT font;
    CreateFont();

    SelectFont(hdc, font);
    GetFontUnicodeRanges();*/

    if (helpRequested_)
        return Application::EXIT_OK;

//     if (args.size() < 3) {
//         displayHelp();
//         return Application::EXIT_USAGE;
//     }

    logInfo(logger(), "input.folder   = " + config().getString("input.folder", ""));
    logInfo(logger(), "output.folder  = " + config().getString("output.folder", ""));
    logInfo(logger(), "mapping.folder = " + config().getString("mapping.folder", ""));
    logInfo(logger(), "working.mode   = " + config().getString("working.mode", "quick"));

    // perform the main task
//     for (int i = 0; i < args.size(); ++i)
//         logInfo(logger(), "args[" + boost::lexical_cast<string_t>(i) + "] = " + args[i]);

    Converter converter(tConfigPtr(&config(), true));
    converter.start();

    return Application::EXIT_OK;
}

POCO_APP_MAIN(DocProcessor)