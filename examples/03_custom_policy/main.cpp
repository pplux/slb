#include <SLB/SLB.hpp>

// To make a Implementation of a Policy:
//    - have a look at SLB/Instance.hpp to see how the Default implementation works
//    - understand what's going on
//    - follow the schema: make a struct with your policy, inside there must be a templated class
//           called Implementation that inhertis from SLB::InstanceBase
//    - adjust the _flags properly:
//			I_Invalid   = Invalid object, the default state. Also is equal to 0x00
//			I_Copy      = The Current object comes from a "copy" of a given object. It can also indicate
//		                  that the object is attached to the lua_state, so when the lua collects the object
//                        the memory should be freed.
//			I_Reference = The object passed was a reference.
//			I_Pointer   = The object passed was a pointer. 
//			I_Const_Pointer = The object passed was a const-pointer (or const-reference)
//

struct CustomPolicy {
	template<class T>
	class Implementation : public virtual SLB::InstanceBase
	{
	public:
		// constructor from a pointer 
		// @fromConstructor is true if this instance was created inside the scripting language calling a 
		//    class constructor method. If this instance was returned by a function, thus was not 
		//    created inside the script then @fromConstructor = false.
		Implementation( T* ptr, bool fromConstructor = false ) : InstanceBase( I_Pointer, typeid(T) ), _ptr(ptr)
		{
			if (fromConstructor) _flags |= I_Copy;
		}
		// constructor from const pointer
		Implementation( const T *ptr ) : InstanceBase( I_Const_Pointer, typeid(T)), _const_ptr(ptr)
		{
		}

		// constructor from reference
		Implementation( T &ref ) : InstanceBase( I_Reference, typeid(T) ), _ptr( &ref )
		{
		}

		// copy constructor,  
		Implementation( const T &ref) : InstanceBase( I_Copy, typeid(T) ), _ptr( 0L )
		{
			_ptr = new T( ref );
		}

		virtual ~Implementation() { if (isCopy()) delete _ptr; }

		void* get_ptr() { return (isConst())? 0L : _ptr; }
		const void* get_const_ptr() { return _const_ptr; }
	protected:
		union {
			T *_ptr;
			const T *_const_ptr;
		};
	};
};

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
