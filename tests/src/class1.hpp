#ifndef __TEST_CLASS1__
#define __TEST_CLASS1__

class Base1
{
public:
	Base1();
	virtual ~Base1();
	float method_b1(float a);

protected:
	float _data[20];
};


class Base2 : public virtual Base1
{
public:
	Base2();
	virtual ~Base2();
	float method_b2(float a);

protected:
	float _data[20];
};

class Class1 : public Base2
{
public:
	Class1(int v = 5);
	virtual ~Class1();
	int method1(float a, float b);
	virtual void method2(float);
	static int methods(float a, float b);

	int method_overloaded(int a);
	float method_overloaded(float a) const;

protected:
	float _value;

};

#endif
