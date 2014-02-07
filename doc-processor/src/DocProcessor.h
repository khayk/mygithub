#ifndef DocProcessor_h__
#define DocProcessor_h__

#include <Poco/Util/Application.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/Option.h>

using Poco::Util::Application;
using Poco::Util::OptionSet;
using Poco::Util::Option;

/// Patcher application
class DocProcessor : public Application {
public:
    DocProcessor();

protected:
    void initialize(Application& self);
    void uninitialize();

    void defineOptions(OptionSet& options);
    void handleHelp(const std::string& name, const std::string& value);
    void displayHelp();

    /// application entry point
    int main(const std::vector<std::string>& args);

private:
    bool helpRequested_;
};

#endif // DocProcessor_h__

