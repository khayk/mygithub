#ifndef AllCommands_h__
#define AllCommands_h__

/// valid commands
#define CMD_ENCODE       L"encode"
#define CMD_COMBINE      L"combine"
#define CMD_ANALYZE      L"analyze"


/// possible arguments
#define ARG_SRC_DIR      L"src-dir"
#define ARG_SRC_FILE     L"src-file"
#define ARG_SRC_ENCODING L"src-encoding"

#define ARG_DST_DIR      L"dst-dir"
#define ARG_DST_FILE     L"dst-file"
#define ARG_NAME_MAP     L"name-mapping"

#define ARG_METHOD       L"method"
#define ARG_WORD         L"word"

#define FORM_ARG(x)		 L"--"##x

#endif // AllCommands_h__
