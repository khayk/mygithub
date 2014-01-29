	#ifndef Utils_h__
	#define Utils_h__


	/// returns application directory
wstring_t getAppDir();

	/// return true if the path specified by input parameter is absolute path
	/// otherwise returns false
bool isAbsolutePath(const wstring_t& str);

	/// return true if the specified string represents an integer number
bool isNumber(const wstring_t& str);

	/// convert string into integer number
	template <typename T>
inline T toNumber( const wstring_t& str ) {
	try {
		return boost::lexical_cast<T>(str);
	}
	catch (const std::exception& e) {
		std::cout << "exception: " << e.what() << std::endl;
	}
	return 0;
}

	/// format conversion
void convertFormat( const wstring_t& uniStr, string_t& multiStr, long codePage );
void convertFormat( const string_t& multiStr, wstring_t& uniStr, long codePage );

string_t toUtf8(const wstring_t& source);
wstring_t toUtf16(const string_t& source);

	/// read from specified file into buffer rawData
bool readFileAsBinary(const wstring_t& fileName, string_t& rawData);
bool writeFileAsBinary(const wstring_t& fileName, string_t& rawData);

wstring_t getFileName(const wstring_t& fileName);
wstring_t getFileExt(const wstring_t& fileName);

	/// trim tailing and leading spaces from the string
wstring_t trim(wstring_t& str);

	#endif // Utils_h__