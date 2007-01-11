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

int main(int, char **)
{
	SLB_DEBUG(0, "Start....");
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	Class1 obj(8);

	execute(L, "c = Cosa(6)");



	SLB_DEBUG(0, "End of test....");
	lua_close(L);
	SLB_DEBUG(0, "End....");
	return 0;
}
