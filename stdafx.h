// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>

#include <process.h>
#include <assert.h>
#include <limits.h>
#include <algorithm>

#include <WinSock2.h>
#include <Mswsock.h>


#include <cstdint>
#include <memory>
#include <deque>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <mutex>

#include "Etc/Singleton.h"

// TODO: reference additional headers your program requires here
