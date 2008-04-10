#include <SLB/SLB.hpp>
#include <iostream>

#ifdef USE_VALGRIND
#include <valgrind/memcheck.h>
#endif

int main(int argc, char **argv)
{
	SLB_DEBUG_CALL;
	SLB_DEBUG(0, "Start test...");
	SLB::Script *s = new SLB::Script();
	SLB_DEBUG(0, "Open SLB...");

	SLB_DEBUG(0, "Loading script...");
	s->doFile(argv[1]);

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

	return 0;
}
