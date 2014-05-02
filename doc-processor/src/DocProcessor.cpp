// doc-processor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DocProcessor.h"
#include "core/Converter.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/File.h>
#include "utils/Common.h"
#include "core/Security.h"

#include <boost/lexical_cast.hpp>
#include "mapping/ASCII_Chars_Unicode.h"

#include <Poco/NumberFormatter.h>
#include <Poco/FileStream.h>
#include <Poco/String.h>

using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

class ChangeLoggerLevel {
public:
    ChangeLoggerLevel(DocProcessor* dp) : dp_(dp) {
        if (dp_)
            logInfo(dp_->logger(), makeCentralizedNote("APPLICATION STARTED", '-', 80));
    }
    ~ChangeLoggerLevel() {
        if (dp_) {
            logInfo(dp_->logger(), makeCentralizedNote("APPLICATION ENDED", '-', 80));
            dp_->logger().setLevel(Poco::Message::PRIO_WARNING);
        }
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

void handleRequests(const string_t& requests) {
    try {
        Poco::Path pp(requests);
        Poco::Path lics = pp.parent().makeDirectory().pushDirectory("licenses");

        Poco::FileInputStream fis(requests);

        while (!fis.eof()) {
            string_t sl;
            string_t sha;
            string_t user;

            std::getline(fis, sl);

            /// check if there is custom mapping file specified current font
            string_t::size_type p = sl.find('|');
            sha = sl.substr(0, p);
            sl = Poco::trim(sl);

            if (p != string_t::npos)
                user = sl.substr(p + 1);

            sha = Poco::trim(sha);
            user = Poco::trim(user);

            if (sha.empty())
                continue;

            Poco::Path tmp = lics;
            tmp.pushDirectory(sha); 
            string_t ss = tmp.toString();

            if ( !Poco::File(ss + LICENSE_FILE).exists() ) {
                SecurityKey sk;
                sk.create(sha, false);

                Poco::File(ss).createDirectories();
                sk.save(ss + LICENSE_FILE);
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {

    }
}

int DocProcessor::main( const std::vector<std::string>& args )
{
    if (!args.empty()) {
        handleRequests(args[0]);
        return 0;
    }

//    sk.save();
//     Security security(tConfigPtr(&config(), true));
//     SecurityKey& key = security.getKey();
//     logContent(key);
//     return 0;
    //timeAsStr(key.getLastUseTime());
// 
//     if ( !key.updateCounters(10010000) ) {
//         logContent(key);
//         return 0;
//     }
//     //logContent(key);
// 
//     return 0;

    ChangeLoggerLevel cll(this);
    wstring_t appVersion;
    string_t appPath = config().getString("application.path");
    getFileVersion(toUtf16(appPath), appVersion);
    logInfo(logger(), "Application version: " + toUtf8(appVersion));

    string_t shaExpected;
    string_t sha1Calculated;
    calculateSHA1( Poco::Path(appPath).parent().append("README.txt").toString(), 
        sha1Calculated );

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
//     AsciiToUniMapping::exportArmenianASCII_Unicode("../config/mapping/armenian/char-mapping-default.txt");
//     AsciiToUniMapping::exportRussianASCII_Unicode("../config/mapping/russian/char-mapping-default.txt");
//    AsciiToUniMapping::createArafi_UnicodeMapping("../config/mapping/armenian/cm-arafi.txt");
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

    if (sha1Calculated != "a128b9f1fb0a7bcce2f2df7b6481beb8f1e36c2c") {
        logError(logger(), "Any change in the README.txt file "
            "should be coordinated with the author "
            "(karapetyan.hayk@gmail.com). Press ENTER to close the window");
        int ch;
        ch = std::cin.get();
        return 0;
    }

    bool errorOccured = true;
    try {
        Converter converter(tConfigPtr(&config(), true));
        converter.start();
        errorOccured = false;
    }
    catch (const Poco::Exception& e) {
        logError(logger(), e.displayText());
    }
    catch (const std::exception& e) {
        logError(logger(), e.what());
    }

    if (errorOccured) {
        logInfo(logger(), "Press ENTER to close the window.");
        int ch;
        ch = std::cin.get();
    }

    return Application::EXIT_OK;
}

POCO_APP_MAIN(DocProcessor)