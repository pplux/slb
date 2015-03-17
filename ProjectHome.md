# Simple Lua Binder v3 (SLB3) #

SLB3 is an "simple-to-use" library to wrap C/C++ and use it from lua scripts. The new implementation (v3) is a complete rewrite of the library, removing everything that wasn't really needed in favor of speed and efficiency.

SLB2 is still maintained, but it will be deprecated in the long run. Most of the features of SLB2 (like Singleton manager, custom typeid information, and so on) has been removed for a more direct way to wrap classes into lua\_States. The new implementation of SLB3 easier to debug, memory efficient, faster, and closer to the kind of wrapper that you will write manually.

SLB3 Features:
  * SLB3 doesn't depend on any library (except lua), it **doesn't** use **boost**
  * should compile fine on every platform.
  * It is written with pre-C++11 compilers in mind.
  * It doesn't allocate memory by itself, if memory is needed it will use the same allocator as lua
  * the API is reentrant and thread-safe
  * Wrapping methods and values follows a syntax similar to boost::python, or Luabind.

Needless to say that SLB3 is **completely** incompatible with SLB2 (to simplify the migration, SLB-v3 uses the namespace SLB3).

SLB3 is the _default_ branch of the repository http://code.google.com/p/slb/source/browse/

SLB2 can be found in the branch _slb2_ of the repository http://code.google.com/p/slb/source/browse/?name=slb2