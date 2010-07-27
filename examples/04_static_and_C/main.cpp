#include <SLB/SLB.hpp>

class StaticClass
{
public: 
	static int getInt() { return _int; }
	static const std::string& getString() { return _string; }
	static void setInt(int i) { _int = i; }
	static void setString(const std::string &s) { _string = s; }

private:
	StaticClass();
	~StaticClass();

	static std::string _string;
	static int _int;
};

std::string StaticClass::_string;
int StaticClass::_int;

void doWrappers()
{
	std::cout << "Loading wrappers..." << std::endl;
	// this will register the wrapper of StaticClass
	SLB::Class< StaticClass, SLB::Instance::NoCopyNoDestroy >("StaticClass")
		// static methods doesn't require de & like memebers
		// see example_01
		.set("getString", StaticClass::getString)
		.set("setString", StaticClass::setString)
		.set("getInt", StaticClass::getInt)
		.set("setInt", StaticClass::setInt)
	;
};

int main(int, char**)
{
	const char *lua_code = 
		"print(SLB.StaticClass)\n"
		"SLB.using(SLB) -- sets SLB methods/classes avialable globally\n"
		"StaticClass.setString('test_string')\n"
		"StaticClass.setInt(5)\n"
		"print('s',StaticClass.getString())\n"
		"print('i',StaticClass.getInt())\n"
		;

	doWrappers();

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Script s;
	std::cout << "SCRIPT CODE:" << std::endl << lua_code << std::endl;
	std::cout << "------------------" << std::endl;
	s.doString(lua_code);

	return 0;
}
