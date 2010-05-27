#include <SLB/SLB.hpp>

int main(int, char**)
{
	const char *lua_code = 
		"print('print SLB functions:')\n"
		"for k,_ in pairs(SLB) do print('SLB.'..k) end\n" 
		"print('---------------')\n";

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Script s;
	s.doString(lua_code);

	/* but you can include SLB features on any lua_State */
	// Create a lua State, using normal lua API
	lua_State *L = luaL_newstate();
	// load default functions (the current example uses print)
	// and by default, SLB::Script does this.
	luaL_openlibs(L);
	// Register SLB inside the lua_State, calling the SLB::Manager
	// that handles bindings, default functions, default values...
	SLB::Manager::getInstance().registerSLB(L);
	// No call lua API to execute the same code as above
	luaL_dostring(L, lua_code); // execute code

	return 0;
}
