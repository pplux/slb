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

Class1::Class1()
{
	SLB_DEBUG(1, "Class1 constructor %p", this);
}

Class1::~Class1()
{
	SLB_DEBUG(1, "Class1 destructor %p", this);
}

int Class1::method1(float a, float b)
{
	SLB_DEBUG(0, "Class1(%p)::method1(%f,%f)",this,a,b);
	int r = (int) ( a / b );
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

void registerClass1()
{
	SLB::Class *c =  0;
	c = SLB::Manager::getInstance().getOrCreateClass(typeid(Class1));
	c->setName("Class1");
	c->set( "method1", SLB::FuncCall::create(&Class1::method1) );
	c->set( "methods", SLB::FuncCall::create(Class1::methods) );
	c->inheritsFrom<Class1, Base1>();
	c->inheritsFrom<Class1, Base2>();

	c = SLB::Manager::getInstance().getOrCreateClass(typeid(Base1));
	c->setName("Base1");
	c->set( "method_b1", SLB::FuncCall::create(&Base1::method_b1) );

	c = SLB::Manager::getInstance().getOrCreateClass(typeid(Base2));
	c->setName("Base2");
	c->set( "method_b2", SLB::FuncCall::create(&Base2::method_b2) );
	c->inheritsFrom<Base2, Base1>();
}

void unregisterClass1()
{
}
