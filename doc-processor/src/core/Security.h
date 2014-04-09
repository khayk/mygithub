#ifndef SECURITY_H__
#define SECURITY_H__

#include "../utils/HwInfo.h"

#include <Poco/Util/AbstractConfiguration.h>
typedef Poco::AutoPtr<Poco::Util::AbstractConfiguration>    tConfigPtr;


// 1 - 356a192b7913b04c54574d18c28d46e6395428ab
// 2 - da4b9237bacccdf19c0760cab7aec4a8359010b0
// 3 - 77de68daecd823babbb58edb1c8e14d7106e83bb


const int64 DAILY_LIMIT = 1024 * 1024;
const int64 BYTES_LIMIT = 50 * DAILY_LIMIT;

const int BLOCK_SZ = 80;
const int KEY_LEN  = 77 * BLOCK_SZ;
const int SHA_LEN  = 40;

class MachineGUID {
public:
    MachineGUID();

    const string_t& getHwId() const;

private:
    string_t hwId_;
};

class SecurityKey {
public:
    SecurityKey();

    void move(const string_t& destination);
    bool load(const string_t& path);
    void save(const string_t& path);
    void create(const string_t& token, bool personal = true);

    string_t getToken() const;
    
    bool     isPersonal() const;
    bool     isGenuine(const string_t& token) const;
    bool     isLimitExceeded() const;
    bool     isDailyLimitExceeded() const;

    int64    getDailyUsage() const;  
    int64    getTotalBytes() const;
    int64    getTotalDocCount() const;
    int64    getLastUseTime() const;
    int64    getDateCreated() const;
    int64    getVersion() const;

    void     setDailyUsage(int64 val);  
    void     setTotalBytes(int64 val);
    void     setTotalDocCount(int64 val);
    void     setLastUseTime(int64 val);
    void     setDateCreated( int64 val );
    void     setVersion(int64 val);

    bool     updateCounters(int64 bytesProcessed);

private:
    int64    readValue(int offset) const;
    void     storeValue(int offset, int64 val);
    
    void initialize();
    void initializeIndicies();
    void extractToken(); /// extract token information from the key
    void injectToken( const string_t& token );
    
    string_t token_;
    string_t key_;
    string_t dst_;

    int      offKeyType_;
    int      offVersion_;
    int      offLastUsageTime_;
    int      offBytesProcessedToday_;
    int      offTotalDocCount_;
    int      offTotalBytesProcessed_;
    int      offDataCreated_;
    short    indicies_[SHA_LEN];
    int      offNumbers_;
};


class Security {
public:
    Security(tConfigPtr cfg);
    ~Security();

    SecurityKey& getKey();

    int64 getRemainingBytes();
    void  dataProcessed(int bytes);
    bool  alreadyCreated() const;

private:
    string_t    selfToken_;   /// this is a token of current machine
    SecurityKey key_;
    MachineGUID hwInfo_;
    //HwInfo      hwInfo_;

    string_t    rootKeyPath_;
    string_t    appDataKeyPath_;
};


void logContent(const SecurityKey& key);
string_t timeAsStr(int64 val);

#endif
