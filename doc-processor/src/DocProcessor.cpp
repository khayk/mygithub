// doc-processor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DocProcessor.h"

#include <Poco/Util/HelpFormatter.h>

using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

DocProcessor::DocProcessor()
    : helpRequested_(false)
{
    setUnixOptions(true);
}

void DocProcessor::initialize( Application& self )
{
    loadConfiguration();   // load default configuration files, if present
    Application::initialize(self);
}

void DocProcessor::uninitialize()
{
    Application::uninitialize();
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

int DocProcessor::main( const std::vector<std::string>& args )
{
    if (helpRequested_)
        return Application::EXIT_OK;

    for (int i = 0; i < args.size(); ++i)
        std::cout << args[i] << std::endl;

    return Application::EXIT_OK;
}

POCO_APP_MAIN(DocProcessor)