#ifndef PATCHER_COMMON_H__
#define PATCHER_COMMON_H__

string_t  toUtf8(const wstring_t& source);
wstring_t toUtf16(const string_t& source);

/// form a string like this :  [XXX...X note XXX...X]
/// where
/// @param ch   [in]  see X above
/// @param note [in]  any text
/// @param len  [in]  number of symbols between [ and ] in the example above
string_t  makeCentralizedNote(const std::string& note, const char ch, size_t len);

/// read context of  fileName file into rawData
///
/// @param fileName [in]  file full path
/// @param rawData  [in]  contains file content in case of success
void readFileAsBinary(const string_t& fileName, string_t& rawData);

/// write context of rawData into specified file
///
/// @param fileName [in]  file full path
/// @param rawData  [in]  contains file content in case of success
void writeFileAsBinary(const string_t& fileName, const string_t& rawData);

/// construct 'thisPath' path relative to 'relativeTo' path. If it is not
/// possible to construct relative path, the 'thisPath' will be returned
/// (for example if two paths are pointing to a different drives)
string_t buildRelativePath(const string_t& thisPath, const string_t& relativeTo);

/// calculates an SHA-1 value for a given file
void calculateSHA1(const string_t& file, string_t& sha1Value);

/// report about problem which occurred during function call
void reportFailure(const string_t& fnName, const string_t& params, HRESULT hr);

#endif // PATCHER_COMMON_H__
