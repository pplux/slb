#include <SLB/SLB.hpp>

class FirstClass
{
public: 
	FirstClass() : _string(), _int(0)
	{
		std::cout << "FirstClass constructor "<< (void*) this << std::endl;
	}

	~FirstClass()
	{
		std::cout << "FirstClass destructor " << (void*) this << std::endl;
	}

	FirstClass(const FirstClass &m) : _string(m._string), _int(m._int)
	{
		std::cout << "FirstClass copy constructor from " << (void*) &m << " -> " << (void*) this << std::endl;
	}

	int getInt() const { return _int; }
	const std::string& getString() const { return _string; }
	void setInt(int i) { _int = i; }
	void setString(const std::string &s) { _string = s; }

private:
	std::string _string;
	int _int;

};

void doWrappers()
{
	std::cout << "Loading wrappers..." << std::endl;
	// this will register the wrapper of FirstClass
	SLB::Class< FirstClass >("FirstClass")
		// a comment/documentation for the class [optional]
		.comment("This is our wrapper of FirstClass class!!")
		// empty constructor, we can also wrapper constructors
		// with arguments using .constructor<TypeArg1,TypeArg2,..>()
		.constructor()
		// a method/function/value...
		.set("getString", &FirstClass::getString)
			// comment of the method [optional]
			.comment("Returns the internal string")
		.set("setString", &FirstClass::setString)
			.comment("sets the internal string")
			// comment of the first parameter [optional]
			.param("the string to copy")
		.set("getInt", &FirstClass::getInt)
			.comment("returns the internal int")
		.set("setInt", &FirstClass::setInt)
			.comment("sets the internal int value")
			.param("the int to copy")
	;
};

int main(int, char**)
{
	const char *lua_code = 
		"print(SLB.FirstClass)\n"
		"c1 = SLB.FirstClass()\n"
		"SLB.using(SLB) -- sets SLB methods/classes avialable globally\n"
		"c2 = FirstClass()\n"
		"c1:setString('c1_string')\n"
		"c2:setString('c2_string')\n"
		"print('c1',c1:getString())\n"
		"print('c2',c2:getString())\n"
		"print'c3 = c2 will copy a reference, *NO* real copy'\n"
		"c3 = c2 \n"
		"print'changeing c2 will affect c3'\n"
		"c2:setString('c2_string_v2')\n"
		"print('c2',c2:getString())\n"
		"print('c3',c3:getString())\n"
		"print'now we copy c2 returning a new instance'\n"
		"c3 = SLB.copy(c2) \n"
		"print'now c3 is a new instance independent from futher changes to c2'\n"
		"c2:setString('c2_string_v3')\n"
		"print('c2',c2:getString())\n"
		"print('c3',c3:getString())\n"
		;

	doWrappers();

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Script s;
	std::cout << "SCRIPT CODE:" << std::endl << lua_code << std::endl;
	std::cout << "------------------" << std::endl;
	s.doString(lua_code);

	return 0;
}
