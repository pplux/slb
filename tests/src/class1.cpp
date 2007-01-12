#include<SLB.hpp>
#include "class1.hpp"

Base1::Base1()
{
	SLB_DEBUG(1, "Base1 constructor %p", this);
}

Base1::~Base1()
{
	SLB_DEBUG(1, "Base1 destructor %p", this);
}

float Base1::method_b1(float a)
{
	SLB_DEBUG(1, "Base1 method_b1 %p", this);
	_data[0] = a*2;
	return _data[0];
}

Base2::Base2()
{
	SLB_DEBUG(1, "Base2 constructor %p", this);
}

Base2::~Base2()
{
	SLB_DEBUG(1, "Base2 destructor %p", this);
}

float Base2::method_b2(float a)
{
	SLB_DEBUG(1, "Base2 method_b2 %p", this);
	_data[0] = a*2;
	return _data[0];
}

Class1::Class1(int v) : _value(v)
{
	SLB_DEBUG(1, "Class1 constructor %p value = %d", this, v);
}

Class1::~Class1()
{
	SLB_DEBUG(1, "Class1 destructor %p", this);
}

int Class1::method_overloaded(int a)
{
	return a*2;	
}
float Class1::method_overloaded(float a) const
{
	return a/2.0;
}

int Class1::method1(float a, float b)
{
	SLB_DEBUG(0, "Class1(%p)::method1(%f,%f)",this,a,b);
	int r = (int) ( a / b  + _value );
	return r;
}

int Class1::methods(float a, float b)
{
	SLB_DEBUG(0, "(static) Class1::methods(%f,%f)",a,b);
	int r = (int) ( a / b );
	return r;
}
void Class1::method2(float)
{
}
/*
SLB_CLASS(Class1)
	SLB_Method(method1)
	SLB_Func("method1", &Class1::method1)
	SLB_Func("methods",  Class1::methods)
	SLB_Func("method_overloaded1", static_cast<int (Class1::*)(int)>(&Class1::method_overloaded))
	SLB_Constructor(int)
SLB_END
*/

SPP_STATIC_BLOCK(Class1Registration, 
	SLB::Class<Class1>("Class1")
		.set( "method1", &Class1::method1 )
		.set( "methods", Class1::methods )
		.set( "method_overloaded1", static_cast<int (Class1::*)(int)>(&Class1::method_overloaded))
		.set( "cst", Class1(7) )
		.constructor<int>()
		.inherits<Base1>()
		.inherits<Base2>()
	;
)
