#include <SLB/SLB.hpp>

class MyClass
{
public: 
	MyClass(const std::string &s, int i) : _string(s), _int(i)
	{
		std::cout << "MyClass constructor "<< (void*) this
			<<" -> " << s << ", " << i << std::endl;
	}

	~MyClass()
	{
		std::cout << "MyClass destructor " << (void*) this << std::endl;
	}

	int getInt() const { return _int; }
	const std::string& getString() const { return _string; }
	void setInt(int i) { _int = i; }
	void setString(const std::string &s) { _string = s; }

private:
	MyClass(const MyClass &m);
	std::string _string;
	int _int;
};

void doWrappers()
{
	std::cout << "Loading wrappers..." << std::endl;
	// MyClass doesn't have a copy-constructor so it can not be
	// wrapped with the default policy, you should use here
	// SLB::Instance::NoCopy
	SLB::Class< MyClass, SLB::Instance::NoCopy >("MyClass")
		// example of a constructor with arguments, in form of
		// C++ template
		.constructor<const std::string&, int>()
		// a method/function/value...
		.set("getString", &MyClass::getString)
		.set("setString", &MyClass::setString)
		.set("getInt", &MyClass::getInt)
		.set("setInt", &MyClass::setInt)
	;

	// SLB instance policies policies (SLB::Instance::)
	// - NoCopy -> doesn't allow copying objects
	// - NoCopyNoDestroy -> NoCopy is allowed, and also doesn't call
	//                      to destructors
	// - SmartPtr<T_sm> -> Typical SmartPointer based object, where
	//                  T_sm<T> can be instantiated (like auto_ptr)
	// - SmartPtrNoCopy<T_sm> -> SmartPointer, with disabled copy
	// - SmartPtrSharedCopy<T_sm> -> SmartPointer, but the copy is 
	//                               based on the copy of T_sm itself
	
};

int main(int, char**)
{
	doWrappers();
	return 0;
}
