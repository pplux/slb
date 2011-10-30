#include <SLB/SLB.hpp>

struct Shape
{
	virtual const char* name() const= 0;
};

struct Circle : public Shape
{
	float radius() const { return 6.0f; }
	const char* name() const { return "circle"; }
};

struct Square: public Shape
{
	float width() const { return 2.0f; }
	const char* name() const { return "square"; }
};

struct MyScript : public SLB::Script
{
public:
	MyScript()
	{
		const char *lua_code = 
		"function foo(shape) \n"
		"  if SLB.type(shape) == 'Circle' then print(shape:name(),'radius=',shape:radius()) end\n"
		"  if SLB.type(shape) == 'Square' then print(shape:name(),'width=',shape:width()) end\n"
		"end\n"
		;
		
		// load the code inside the script
		doString(lua_code);
	}

	void invokeFooMethod(Shape *s)
	{
		lua_State *L = getState();
		// prepare a call object
		SLB::LuaCall<void(Shape*)> call(L,"foo");
		// perform the call with the parameters
		call(s);
	}
};

void doWrappers()
{
	std::cout << "Loading wrappers..." << std::endl;
	SLB::Class<Circle>("Circle")
		.constructor()
		.inherits<Shape>()
		.set("radius", &Circle::radius)
		;
	SLB::Class<Square>("Square")
		.constructor()
		.inherits<Shape>()
		.set("width", &Square::width)
		;
	//Shapes can not be copied from the "abstract" base class
	SLB::Class<Shape, SLB::Instance::NoCopy>("Shape")
		.set("name", &Shape::name)
		;
};

void example() {
  MyScript s;
	Circle circle;
	Square square;
	s.invokeFooMethod(&circle);
	s.invokeFooMethod(&square);
}

int main(int, char**)
{
	doWrappers();
#if SLB_USE_EXCEPTIONS
	try
	{
	  example();
	}
	catch(std::exception &e)
	{
		std::cerr << "ERROR:" << e.what();
	}
#else
  example();
#endif

	return 0;
}
