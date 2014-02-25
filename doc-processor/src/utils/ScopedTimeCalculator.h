#ifndef SCOPED_TIME_CALCULATOR_H__
#define SCOPED_TIME_CALCULATOR_H__

#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>

class ScopedTimeCalculator
{
public:
    ScopedTimeCalculator(Poco::Logger& logger, const string_t& taskName);
    ~ScopedTimeCalculator();

private:
    Poco::DateTime  startTime_;
    Poco::Logger&   logger_;
    string_t        taskName_;
};


#endif