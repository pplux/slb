#include<SLB.hpp>
#include "class1.hpp"
#include <iostream>

void execute(lua_State *L, const char *command)
{
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "- Execute : " << command << std::endl << std::endl;
	if (luaL_dostring(L, command))
	{
		std::cout << "ERROR... " << lua_tostring(L, -1) << std::endl;
		lua_pop(L,1);
	}
	std::cout << "-----------------------------------------" << std::endl;
}

int main(int argc, char **argv)
{
	SLB_DEBUG(0, "Start....");
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	SLB::Manager::getInstance().registerSLB(L);

	Class1 obj(8);

	for (int i = 1; i < argc; ++i) execute(L, argv[i]);

	SLB_DEBUG(0, "End of test....");
	lua_close(L);
	SLB_DEBUG(0, "End....");
	return 0;
}
