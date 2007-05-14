#include<SLB.hpp>
#include "class1.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	SLB_DEBUG(0, "Start test...");
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	SLB_DEBUG(0, "Open SLB...");
	SLB::Manager::getInstance().registerSLB(L);

	SLB_DEBUG(0, "Loading script...");
	if( luaL_dofile(L, argv[1]) )
	{
		std::cerr << "Error: " << lua_tostring(L,-1) << std::endl;
		exit(1);
	}

	SLB_DEBUG(0, "Closing script...");
	lua_close(L);
	SLB_DEBUG(0, "End Test...");
	exit(0);
}
