#include<SLB.hpp>
#include "class1.hpp"
#include <iostream>

void execute(lua_State *L, const char *command)
{
	if (luaL_dostring(L, command))
	{
		std::cout << "ERROR... " << lua_tostring(L, -1) << std::endl;
		lua_pop(L,1);
	}
}

int main(int, char **)
{
	SLB_DEBUG(0, "Start....");
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	SLB_DEBUG(0, "OpenLibs....");

	SLB_DEBUG(0, "Register Class1....");
	registerClass1();

	Class1 obj;

	SLB::ref_ptr<SLB::Table> table = new SLB::Table("/",true);
	table->set("tmp/tmp2/tmp3/obj1", SLB::Value::autoDelete(new Class1));
	table->push(L);
	lua_setglobal(L,"SLB");
	execute(L, "SLB.tmp.tmp2.tmp3.obj1:method1(50,10)");	
	execute(L, "SLB.tmp.tmp2.tmp3.obj1.methods(50,5)");	
	SLB::push(L, &obj);
	lua_setglobal(L, "obj");
	execute(L, "obj:method1(5,6)");
	execute(L, "obj:method_b2(5)");


	SLB_DEBUG(0, "End of test....");
	lua_close(L);
	SLB_DEBUG(0, "Unregister Class1....");
	unregisterClass1();
	SLB_DEBUG(0, "End....");
	return 0;
}
