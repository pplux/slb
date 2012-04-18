/*
    SLB - Simple Lua Binder
    Copyright (C) 2007-2010 Jose L. Hidalgo Valiño (PpluX)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
	
	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/


#include <SLB/SLB.hpp>
#include <iostream>

#if defined(_WIN32) && defined(USE_VALGRIND)
#undef USE_VALGRIND
#endif

#ifdef USE_VALGRIND
#include <valgrind/memcheck.h>
#endif

struct TestScript : public SLB::Script {
  virtual void onNewState(lua_State *L) {
    lua_register(L, "probe", TestScript::script_probe);
  }

  static int script_probe(lua_State *L)
  {
    const char *arg = lua_tostring(L,1);

    if (strcmp(arg, "show_all_classes") == 0) {
      std::cout << "SHOW ALL CLASSES" << std::endl;
      const SLB::Manager *m = SLB::Manager::getInstance(L);
      const SLB::Manager::ClassMap &map = m->getClassMap();
      for(SLB::Manager::ClassMap::const_iterator i = map.begin(); i != map.end(); ++i) 
      {
        std::cout << "Class TIW=|" << i->first.name() << "|" << std::endl;
        std::cout << "\t realname |" << i->second->getName() <<"|"<< std::endl;
      }
      return 0;
    }

    luaL_error(L, "Invalid call to probe, valid arguments are"
      "\n\tshow_all_classes"
      );
    return 0;
  }
};

int main(int argc, char **argv)
{
	int result = 0;
	SLB_DEBUG_CALL;
	SLB_DEBUG(0, "Start test...");
	TestScript *s = new TestScript();
	SLB_DEBUG(0, "Open SLB...");
  SLB_DEBUG(0, "Loading script...");
#if SLB_USE_EXCEPTIONS
	try
	{
		if (argc == 2)
		{
			s->doFile(argv[1]);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "SLB_test -> EXCEPTION: " << e.what() << std::endl;
		result = 1;
	}
#else
  if (argc == 2)
	{
    if(!s->safeDoFile(argv[1])) {
      std::cerr << "ERROR:" << s->getLastError() << std::endl;
      result = 1;
    }
	}
#endif

	SLB_DEBUG(0, "Closing script...");
	delete s;
	SLB_DEBUG(0, "Removing manager...");
	SLB::Manager::destroyDefaultManager();
	SLB_DEBUG(0, "End Test...");


#ifdef USE_VALGRIND
//Thanks to:  http://thread.gmane.org/gmane.comp.debugging.valgrind/6504/focus=6505
	if (RUNNING_ON_VALGRIND) {
		VALGRIND_DO_LEAK_CHECK;
		long vg_leaks = 0, vg_dubious = 0, vg_reachable = 0, dummy;
		VALGRIND_COUNT_LEAKS(vg_leaks, vg_dubious, vg_reachable, dummy);

		//if (vg_leaks || vg_dubious || vg_reachable) exit(1);
		if (vg_leaks || vg_dubious) result=2;
	}
#endif

	return result;
}
