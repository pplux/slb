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
	SLB_DEBUG(0, "OpenLibs....");

	SLB_DEBUG(0, "Register Class1....");
	registerClass1();

	Class1 obj(8);

	SLB::ref_ptr<SLB::Table> table = new SLB::Table("::",true);
	table->set("Class1", SLB::Value::autoDelete(new Class1(2)));
	table->push(L);
	lua_setglobal(L,"SLB");
	execute(L, "c = SLB.Class1(6)");



	SLB_DEBUG(0, "End of test....");
	lua_close(L);
	SLB_DEBUG(0, "Unregister Class1....");
	unregisterClass1();
	SLB_DEBUG(0, "End....");
	return 0;
}
