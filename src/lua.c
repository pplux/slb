#define luaall_c


#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#ifndef SLB_STATIC_LIBRARY
		#define LUA_BUILD_AS_DLL
		#ifdef SLB_LIBRARY
			#define LUA_LIB
			#define LUA_CORE
		#endif
	#endif
#endif

#include "lua/lobject.c"
#include "lua/lapi.c"
#include "lua/lcode.c"
#include "lua/ldebug.c"
#include "lua/ldo.c"
#include "lua/ldump.c"
#include "lua/lfunc.c"
#include "lua/lgc.c"
#include "lua/llex.c"
#include "lua/lmem.c"
#include "lua/lopcodes.c"
#include "lua/lparser.c"
#include "lua/lstate.c"
#include "lua/lstring.c"
#include "lua/ltable.c"
#include "lua/ltm.c"
#include "lua/lundump.c"
#include "lua/lvm.c"
#include "lua/lzio.c"

#include "lua/lauxlib.c"
#include "lua/lbaselib.c"
#include "lua/ldblib.c"
#include "lua/liolib.c"
#include "lua/linit.c"
#include "lua/lmathlib.c"
#include "lua/loadlib.c"
#include "lua/loslib.c"
#include "lua/lstrlib.c"
#include "lua/ltablib.c"
