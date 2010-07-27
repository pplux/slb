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

#ifdef USE_VALGRIND
#include <valgrind/memcheck.h>
#endif

int main(int argc, char **argv)
{
	int result = 0;
	SLB_DEBUG_CALL;
	SLB_DEBUG(0, "Start test...");
	SLB::Script *s = new SLB::Script();
	SLB_DEBUG(0, "Open SLB...");

	try
	{
		if (argc == 2)
		{
			SLB_DEBUG(0, "Loading script...");
			s->doFile(argv[1]);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
		result = 1;
	}

	SLB_DEBUG(0, "Closing script...");
	delete s;
	SLB_DEBUG(0, "End Test...");
	SLB::Manager::getInstance().reset(); // clear all memory


#ifdef USE_VALGRIND
//Thanks to:  http://thread.gmane.org/gmane.comp.debugging.valgrind/6504/focus=6505
	if (RUNNING_ON_VALGRIND) {
		VALGRIND_DO_LEAK_CHECK;
		long vg_leaks = 0, vg_dubious = 0, vg_reachable = 0, dummy;
		VALGRIND_COUNT_LEAKS(vg_leaks, vg_dubious, vg_reachable, dummy);

		if (vg_leaks || vg_dubious || vg_reachable) exit(1);
	}
#endif

	return result;
}
