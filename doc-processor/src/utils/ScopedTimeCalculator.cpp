#include "StdAfx.h"
#include "ScopedTimeCalculator.h"
#include <Poco/NumberFormatter.h>
#include <Poco/Logger.h>
    
ScopedTimeCalculator::ScopedTimeCalculator(Poco::Logger& logger, 
    const string_t& taskName)
    : logger_(logger)
    , startTime_()
    , taskName_(taskName)
{
}

ScopedTimeCalculator::~ScopedTimeCalculator()
{
    try {
        Poco::Timespan workingTime = Poco::DateTime() - startTime_;
        string_t workingTimeStr = taskName_ + string_t(" time: ");

        if (workingTime.hours() > 0)
            workingTimeStr += Poco::NumberFormatter::format(workingTime.hours()) + " hours, ";

        if (workingTime.minutes() > 0)
            workingTimeStr += Poco::NumberFormatter::format(workingTime.minutes()) + " minutes, ";

        if (workingTime.seconds() >= 0)
            workingTimeStr += Poco::NumberFormatter::format(workingTime.seconds()) + " seconds.";

        /// insert total seconds information
        if (workingTime.totalSeconds() > 60)
            workingTimeStr += (" Total " + Poco::NumberFormatter::format(workingTime.totalSeconds()) + " seconds.");

        logger_.notice(workingTimeStr);
    }
    catch (...) {
        logger_.error("Something went wrong in DESTRUCTOR!!!");
    }
}
