/* one.c -- Lua core, libraries, and interpreter in a single file
*/

#define MAKE_LIB  1

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
       #ifndef SLB_STATIC_LIBRARY
               #define LUA_BUILD_AS_DLL
               #ifdef SLB_LIBRARY
                       #define LUA_LIB
                       #define LUA_CORE
               #endif
       #endif
#endif


/* default is to build the full interpreter */
#ifndef MAKE_LIB
#ifndef MAKE_LUAC
#ifndef MAKE_LUA
#define MAKE_LUA
#endif
#endif
#endif

/* choose suitable platform-specific features */
/* some of these may need extra libraries such as -ldl -lreadline -lncurses */
#if 0
#define LUA_USE_LINUX
#define LUA_USE_MACOSX
#define LUA_USE_POSIX
#define LUA_ANSI
#endif

#define luaall_c

/* core -- used by all */
#include "lua/lapi.c"
#include "lua/lcode.c"
#include "lua/lctype.c"
#include "lua/ldebug.c"
#include "lua/ldo.c"
#include "lua/ldump.c"
#include "lua/lfunc.c"
#include "lua/lgc.c"
#include "lua/llex.c"
#include "lua/lmem.c"
#include "lua/lobject.c"
#include "lua/lopcodes.c"
#include "lua/lparser.c"
#include "lua/lstate.c"
#include "lua/lstring.c"
#include "lua/ltable.c"
#include "lua/ltm.c"
#include "lua/lundump.c"
#define pushclosure pushclosure2
#include "lua/lvm.c"
#include "lua/lzio.c"
#undef pushclosure

/* auxiliary library -- used by all */
#include "lua/lauxlib.c"

/* standard library  -- not used by luac */
#ifndef MAKE_LUAC
#include "lua/lbaselib.c"
#include "lua/lbitlib.c"
#include "lua/lcorolib.c"
#include "lua/ldblib.c"
#include "lua/liolib.c"
#include "lua/lmathlib.c"
#include "lua/loadlib.c"
#include "lua/loslib.c"
#include "lua/lstrlib.c"
#include "lua/ltablib.c"
#include "lua/linit.c"
#endif

/* lua */
#ifdef MAKE_LUA
#include "lua/lua.c"
#endif

/* luac */
#ifdef MAKE_LUAC
#include "lua/print.c"
#include "lua/luac.c"
#endif
