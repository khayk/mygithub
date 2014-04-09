// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <ObjBase.h>

// TODO: reference additional headers your program requires here
#include <iostream>
#include <string>
#include <set>

typedef std::string                       string_t;
typedef std::wstring                      wstring_t;

typedef std::set<string_t>                tStringSet;

typedef __int64 int64;
const string_t LICENSE_FILE = "license.dat";

#include <map>

#define SECURITY_ENABLED

