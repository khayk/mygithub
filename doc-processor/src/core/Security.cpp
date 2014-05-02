#include "StdAfx.h"
#include "Security.h"
#include "../utils/Common.h"
#include "../RootLogger.h"

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/FileStream.h>
#include <Poco/Util/WinRegistryKey.h>
#include <Poco/DateTimeFormatter.h>


#include <sstream>

const int64 SECOND_PER_DAY = 60 * 60 * 24;
const string_t MY_TRACE = "HKEY_CURRENT_USER\\Software\\1fde5499-c2c7-436e-4444-eab245374df1";

MachineGUID::MachineGUID()
{
    std::stringstream ss;
    int CPUInfo[4];
    __cpuid(CPUInfo, 0);
    for (int i = 0; i < 4; i++)
    {
        ss << CPUInfo[i];
    }

    hwId_ = ss.str();
}

const string_t& MachineGUID::getHwId() const
{
    return hwId_;
}

bool isPrime(int n) {
    bool prime = true;
    for (int j=2; j*j<=n; j++) {
        if (n % j == 0) {
            prime = false;
            break;    
        }
    }   
    return prime;
}

string_t bytesToStr(int64 bytes) {
    const int64 ONE_KB = 1024;
    const int64 ONE_MB = 1024 * ONE_KB;
    const int64 ONE_GB = 1024 * ONE_MB;

    string_t str = Poco::NumberFormatter::format(bytes) + " bytes.";
    if ( bytes / ONE_GB > 0 ) {
        str += " (";
        str += Poco::NumberFormatter::format(bytes / ONE_GB);
        str += " Gb)";
    }
    else if ( bytes / ONE_MB > 0 ) {
        str += " (";
        str += Poco::NumberFormatter::format(bytes / ONE_MB);
        str += " Mb)";
    }
    else if ( bytes / ONE_KB > 0 ) {
        str += " (";
        str += Poco::NumberFormatter::format(bytes / ONE_KB);
        str += " Kb)";
    }
    return str;
}

void logContent( const SecurityKey& key )
{
    tLogger& l = tLogger::root();
    logInfo(l, "License:     " + string_t(key.isPersonal() ? "PERSONAL" : "COMMERCIAL") );
    logInfo(l, "Today usage: " + bytesToStr(key.getDailyUsage()));
    logInfo(l, "Total usage: " + bytesToStr(key.getTotalBytes()));
    logInfo(l, "Last usage:  " + timeAsStr(key.getLastUseTime()));
    logInfo(l, "Total docs:  " + Poco::NumberFormatter::format(key.getTotalDocCount()));
}

string_t timeAsStr( int64 val )
{
    return Poco::DateTimeFormatter::format(Poco::Timestamp::fromEpochTime(val), "%Y %b %e");
}

SecurityKey::SecurityKey()
{
    initialize();
}

void SecurityKey::move( const string_t& destination )
{
    if (dst_ != destination) {
        Poco::File(dst_).copyTo(destination);
        dst_ = destination;
    }
}

bool SecurityKey::load( const string_t& path )
{
    try {
        if (!Poco::File(path).exists())
            return false;

        Poco::FileInputStream fis(path);
        key_.clear();
        while (fis) {
            string_t ln;
            getline(fis, ln);
            key_ += ln;
        }

        extractToken();
        int64 timeNow = time(0);
        int64 lastUsageTime = getLastUseTime();
        if ( timeNow - lastUsageTime > SECOND_PER_DAY ) {
            setDailyUsage( 0 );
        }
        dst_ = path;
        getVersion();
        return true;
    }
    catch (const Poco::Exception& ) {
        logError(tLogger::root(), "Failed to load key file.");
    }
    return false;
}

void SecurityKey::save( const string_t& path )
{
    std::stringstream ss;
    int off = 0;
    while (off < KEY_LEN) {
        ss << key_.substr(off, BLOCK_SZ) << std::endl;
        off += BLOCK_SZ;
    }

    writeFileAsBinary(path, ss.str());
    dst_ = path;
}

void SecurityKey::create( const string_t& token, bool personal /*= true*/ )
{
    srand((unsigned int)time(0));
    key_.reserve(KEY_LEN);
    const char* a = "0123456789abcdef";
    for (size_t i = 0; i < KEY_LEN; ++i) {
        key_.push_back(a[rand() % 16]);
    }

    if (token.length() != SHA_LEN)
        throw std::logic_error("Invalid token is specified.");

    injectToken(token);
    int64 val;
    if (!personal)
        val = (int)'0' + 2 * (rand() % 5) + 1;
    else
        val = (int) '0' + 2 * (rand() % 5);

    storeValue(offKeyType_, val);
    setDailyUsage(0);
    setLastUseTime(time(0));
    setTotalBytes(0);
    setTotalDocCount(0);
    setDateCreated(time(0));
    setVersion(1005);
}

string_t SecurityKey::getToken() const
{
     return token_;
}

bool SecurityKey::isPersonal() const
{
    return (readValue(offKeyType_) % 2 == 0);
}

bool SecurityKey::isGenuine( const string_t& token ) const
{
    return (token == token_);
}

bool SecurityKey::isLimitExceeded() const
{
    if ( !isPersonal() )
        return false;
    return getTotalBytes() > BYTES_LIMIT;
}

bool SecurityKey::isDailyLimitExceeded() const
{
    return getDailyUsage() > DAILY_LIMIT; 
}

int64 SecurityKey::getDailyUsage() const
{
    return readValue(offBytesProcessedToday_);
}

int64 SecurityKey::getTotalDocCount() const
{
    return readValue(offTotalDocCount_);
}

int64 SecurityKey::getLastUseTime() const
{
    return readValue(offLastUsageTime_);
}

int64 SecurityKey::getDateCreated() const
{
    return readValue(offDataCreated_);
}

int64 SecurityKey::getVersion() const
{
    return readValue(offVersion_);
}

int64 SecurityKey::getTotalBytes() const
{
    return readValue(offTotalBytesProcessed_);
}

bool SecurityKey::updateCounters( int64 bytesProcessed )
{
    if (isLimitExceeded()) {
        if (isDailyLimitExceeded()) {
            string_t remTimeStr;

            Poco::Timespan workingTime(long(SECOND_PER_DAY - (time(0) - getLastUseTime())), 0);
            if (workingTime.hours() > 0)
                remTimeStr += Poco::NumberFormatter::format(workingTime.hours()) + " hours, ";

            if (workingTime.minutes() > 0)
                remTimeStr += Poco::NumberFormatter::format(workingTime.minutes()) + " minutes, ";

            if (workingTime.seconds() >= 0)
                remTimeStr += Poco::NumberFormatter::format(workingTime.seconds()) + " seconds";
            logError(tLogger::root(), "Limit exceeded. Come back after " + remTimeStr +
                " or contact for COMMERCIAL license. See README.txt");
            return false;
        }
    }

    if (bytesProcessed != 0) {
        setTotalDocCount( getTotalDocCount() + 1 );
        setTotalBytes( getTotalBytes() + bytesProcessed );
        setDailyUsage( getDailyUsage() + bytesProcessed );
        setLastUseTime( time(0) );
    }

    save(dst_);
    return true;
}

int64 SecurityKey::readValue( int offset ) const
{
    if (offset >= key_.size()) {
        return 0;
    }

    int ln = Poco::NumberParser::parse(key_.substr(offset, 1));
    return Poco::NumberParser::parseHex64(key_.substr(offset + 1, ln));
}

void SecurityKey::storeValue( int offset, int64 val )
{
    string_t valStr = Poco::NumberFormatter::formatHex(val);
    int sz = valStr.size();
    string_t szStr = Poco::NumberFormatter::format(sz);

    key_.replace(offset,      1, szStr);
    key_.replace(offset + 1, sz, valStr);
}

void SecurityKey::extractToken()
{
    token_.resize(SHA_LEN);
    for (int i = 0; i < SHA_LEN; ++i)
        token_[i] = key_[indicies_[i]];
}

void SecurityKey::initializeIndicies()
{
    int count = 0;
    int i = 1;
    while (count < SHA_LEN) {
        if ( isPrime(i) ) {
            indicies_[count] = i + offNumbers_;
            ++count;
        }
        ++i;
    }
}

void SecurityKey::initialize()
{
    key_.reserve(KEY_LEN);
    offNumbers_             = 0;

    initializeIndicies();
    offKeyType_             = indicies_[SHA_LEN - 1] + 37;
    offLastUsageTime_       = offKeyType_ + 10;
    offBytesProcessedToday_ = offKeyType_ + 20;
    offTotalDocCount_       = offKeyType_ + 30;
    offTotalBytesProcessed_ = offKeyType_ + 40;
    offDataCreated_         = offKeyType_ + 50;
    offVersion_             = offKeyType_ + 60;
}

void SecurityKey::injectToken( const string_t& token )
{
//     for (int i = 0; i < key_.size(); ++i) {
//         key_[i] = '.';
//     }
    for (int i = 0; i < SHA_LEN; ++i)
        key_[indicies_[i]] = token[i];
    token_ = token;
}

void SecurityKey::setDailyUsage( int64 val )
{
    storeValue(offBytesProcessedToday_, val);
}

void SecurityKey::setTotalBytes( int64 val )
{
    storeValue(offTotalBytesProcessed_, val);
}

void SecurityKey::setTotalDocCount( int64 val )
{
    storeValue(offTotalDocCount_, val);
}

void SecurityKey::setLastUseTime( int64 val )
{
    storeValue(offLastUsageTime_, val);
}

void SecurityKey::setDateCreated( int64 val )
{
    storeValue(offDataCreated_, val);
}

void SecurityKey::setVersion( int64 val )
{
    return storeValue(offVersion_, val);
}





Security::Security(tConfigPtr cfg)
{
    class ScopedReport {
    public:
        ScopedReport(const SecurityKey& key) : key_(key) {}
        ~ScopedReport() {
            tLogger& l = tLogger::root();
            logInfo(l, string_t("License: ") + 
                (key_.isPersonal() ? "PERSONAL" : "COMMERCIAL"));
            
//             if (key_.isPersonal()) {
//                 logInfo(l, "Daily limit will be applied if the global limit exceeded.");
//                 logInfo(l, "Global limit: " + bytesToStr(BYTES_LIMIT));
//                 logInfo(l, "Daily  limit: " + bytesToStr(DAILY_LIMIT));
//             }
        }
    private:
        const SecurityKey& key_;
    };

    ScopedReport scopedReport(key_);

    string_t hwIdStr = hwInfo_.getHwId();
    calculateSHA1_Str(hwIdStr, selfToken_);
    logInfo(tLogger::root(), "TOKEN: " + selfToken_);

    appDataKeyPath_ = getAppData("", "AsciiToUnicode");
    rootKeyPath_ = cfg->getString("application.dir");

    Poco::File(appDataKeyPath_).createDirectories();

    appDataKeyPath_ += LICENSE_FILE;
    rootKeyPath_    += LICENSE_FILE;

    if ( key_.load(appDataKeyPath_) ) {
        if (key_.getToken() == selfToken_) {
            if (!key_.isPersonal()) {
                /// everything is ok, working with genuine token
                /// and commercial license
                return;
            }
        }
    }

    SecurityKey tmp;
    if ( tmp.load(rootKeyPath_) && 
        tmp.getToken() == selfToken_ && 
        !tmp.isPersonal() ) {
            tmp.move(appDataKeyPath_);
            key_ = tmp; 
    }
    else {
        /// remaining cases are
        /// 1. working with genuine and personal key 
        /// 2. working with stolen key

        if ( !key_.isGenuine(selfToken_) ) {
            if ( alreadyCreated() ) {
                throw std::exception("Cheating attempt.");
            }
            key_.create(selfToken_);
            key_.save(appDataKeyPath_);

            Poco::Util::WinRegistryKey myTrace(MY_TRACE);
            myTrace.setString("", "2");
            return;
        }

        /// working with personal key
        if ( !key_.isGenuine(selfToken_) )
            throw ("Should never happen.");
    }
}

Security::~Security()
{
}

SecurityKey& Security::getKey()
{
    return key_;
}

bool Security::alreadyCreated() const
{
    Poco::Util::WinRegistryKey myTrace(MY_TRACE);
    if (myTrace.exists()) {
        if (myTrace.getString("") == "1")
            return false;
        return true;
    }
    return false;
}
