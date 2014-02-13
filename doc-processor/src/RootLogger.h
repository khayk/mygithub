
#ifndef ROOT_LOGGER_H
#define ROOT_LOGGER_H

#include <Poco/Logger.h>
#include <Poco/LogStream.h>

typedef Poco::Logger tLogger;

#define loggingBlock(l, priority, text)                         \
    if ( l.priority() ) {                                       \
        l.priority(text);                                       \
    }

#define logTrace(lr, msg)           loggingBlock((lr), trace, msg)
#define logDebug(lr, msg)           loggingBlock((lr), debug, msg)
#define logInfo(lr, msg)            loggingBlock((lr), information, msg)
#define logNotice(lr, msg)          loggingBlock((lr), notice, msg)
#define logWarning(lr, msg)         loggingBlock((lr), warning, msg)
#define logError(lr, msg)           loggingBlock((lr), error, msg)
#define logCritical(lr, msg)        loggingBlock((lr), critical, msg)
#define logFatal(lr, msg)           loggingBlock((lr), fatal, msg)

/// derive from this class to have a reference to named logger
class LogSource {
public:
    /// Initializes an instance of the class by the specified log source name.
    LogSource(const std::string& name) : logger_(&tLogger::get(name)) {}

    /// Gets logger object associated with this instance.
    tLogger& logger() const { return *logger_; }

private:
    tLogger mutable* logger_;
};


#endif /// ROOT_LOGGER_H
