#ifndef __SLB_MACROS__
#define __SLB_MACROS__

// Starts a new class Definition
#define SLB_CLASS_BEGIN(Class) SLB_CLASS_BEGIN_N(Class, #Class)

// Starts a new class Definition where the "Name" of the bindec
// class is different from the class itself. For examle this is useful
// to bind the class MyTemplate<Instance> with a valid name :)
#define SLB_CLASS_BEGIN_N(Class, Name)                                               \
		namespace {                                                                  \
		static struct SPP_LINEID(Class_Declaration) {                                \
			const char *SLB_ClassName;                                               \
			SPP_LINEID(Class_Declaration)() {                                        \
				typedef Class SLB_Declaration_Type;                                  \
				SLB_ClassName = Name;                                                \
				SLB::Manager::get().declaration<SLB_Declaration_Type>(Name)


#define SLB_ENUM_BEGIN(BaseType, EnumType) \
		namespace {                        \
		static struct SPP_LINEID(Namespace_Declaration) {  \
			SPP_LINEID(Namespace_Declaration)() {          \
				typedef BaseType SLB_Declaration_Type;     \
				SLB::Namespace *_namespace =        \
				    new SLB::Namespace();           \
				SLB::Manager::get().                \
				    setObject(_namespace, \
						std::string(#BaseType "::" #EnumType)); \
				(*_namespace)

// Ends definition
#define SLB_END                     \
		; } /* end class */         \
		} SPP_LINEID(__Dummy__) ; }


#define SLB_Item( Item ) SLB_Item_N( Item, #Item )
#define SLB_Item_N( Item, Name ) .setValue<int>( SLB_Declaration_Type::Item, Name )

// Adds a class method
#define SLB_AddMethod( Method ) SLB_AddMethod_N( Method , #Method) 

// Adds a class method (disambiguation)
#define SLB_AddConstMethod( Method ) SLB_AddConstMethod_N( Method , #Method) 
// Adds a class method (disambiguation)
#define SLB_AddNonConstMethod( Method ) SLB_AddNonConstMethod_N( Method , #Method) 

// Adds a class method changing the name. SLB can not handle overload so it 
// is illegal to define two methods with the same name.
#define SLB_AddMethod_N( Method , Name) \
	.addMethod(SLB::FuncCall::create(&SLB_Declaration_Type::Method),Name)

#define SLB_AddConstMethod_N( Method , Name) \
	.addMethod(SLB::FuncCall::createConst(&SLB_Declaration_Type::Method),Name)

#define SLB_AddNonConstMethod_N( Method , Name) \
	.addMethod(SLB::FuncCall::createNonConst(&SLB_Declaration_Type::Method),Name)

// Adds a method based on the type of the parameters. For example if the
// method signature is -> float MyClass::method(float x, float y) <- you
// call SLB_AddTypedMethod(method, float, float).
// This macro is useful to solve disambiguations when a method is overloaded.
#define SLB_AddTypedMethod( Method, ReturnedT, ... ) \
	SLB_AddTypedMethod_N( Method , #Method, ReturnedT, __VA_ARGS__) 

// disambiguation
#define SLB_AddTypedConstMethod( Method, ... ) \
	SLB_AddTypedConstMethod_N( Method , #Method, __VA_ARGS__) 

// disambiguation
#define SLB_AddTypedNonConstMethod( Method, ... ) \
	SLB_AddTypedConstMethod_N( Method , #Method, __VA_ARGS__) 

// Adds a method based on its parameters, changin the name. 
#define SLB_AddTypedMethod_N( Method , Name, RT, ...) \
	.addMethod(SLB::FuncCall::create \
	(   (RT (SLB_Declaration_Type::*)( __VA_ARGS__ )) &SLB_Declaration_Type::Method),Name)

#define SLB_AddTypedConstMethod_N( Method , Name, RT, ...) \
	.addMethod(SLB::FuncCall::createConst((RT (SLB_Declaration_Type::*)( __VA_ARGS__ ))&SLB_Declaration_Type::Method),Name)

#define SLB_AddTypedNonConstMethod_N( Method , Name, RT, ...) \
	.addMethod(SLB::FuncCall::createNonConst((RT (SLB_Declaration_Type::*)( __VA_ARGS__ ))&SLB_Declaration_Type::Method),Name)

// Adds a function to the class. In lua this function can be used as a method
// or as a class method. For example, we add a function "func1", lets 
// assume we have in lua a variable object of this class, we can write:
//     object:func(param1, param2)
//     object.func(object, param1, param2)
// Both calls are exactly the same. That means if you register a function into
// a class it can be used as a method in the script languaje or as a static
// class method.
#define SLB_AddFunc( Func) SLB_AddFunc_N(Func, #Func)

// Adds a function to the class changing the name.
#define SLB_AddFunc_N( Func, Name) \
	.addMethod(SLB::FuncCall::create(Func),Name)

// Adds a function as a metamethod. The only metamethod that can not be
// used is "__index"
#define SLB_AddMetaFunc( Func, Name) \
	.addMetaMethod(SLB::FuncCall::create(Func),Name)

// Adds a class method as metamethod. 
#define SLB_AddMetaMethod( Method , Name) \
	.addMetaMethod(SLB::FuncCall::create(&SLB_Declaration_Type::Method),Name)

// Adds a static method. 
#define SLB_AddStaticMethod( Method ) \
	SLB_AddStaticMethod_N( Method, #Method )

#define SLB_AddStaticTypedMethod( Method, ... ) \
	SLB_AddStaticTypedMethod_N(Method, #Method, __VA_ARGS__ )

#define SLB_AddStaticMethod_N( Method, Name) \
	.addStaticMethod(SLB::FuncCall::create(&SLB_Declaration_Type::Method),Name)

#define SLB_AddStaticTypedMethod_N( Method, Name, RT, ...) \
	.addStaticMethod(SLB::FuncCall::create((RT (SLB_Declaration_Type::*)( __VA_ARGS__ ))&SLB_Declaration_Type::Method),Name)

// Adds a Lua function
#define SLB_AddLuaMethod( Func, Name ) \
	.addMethod(new SLB::LuaFuncCall( Func ) , Name)

#define SLB_AddStaticLuaMethod( Func, Name ) \
	.addStaticMethod(new SLB::LuaFuncCall( Func ) , Name)

#define SLB_DefaultDestructor()  .setDefaultDestructor<SLB_Declaration_Type>()

#define SLB_Inherits( Class )    .inheritsFrom< Class >()

#define SLB_DefaultAdd() SLB_AddMetaFunc(SLB::Operator<SLB_Declaration_Type>::defaultAdd, "__add")
#define SLB_DefaultSub() SLB_AddMetaFunc(SLB::Operator<SLB_Declaration_Type>::defaultSub, "__sub")
#define SLB_DefaultMult() \
	SLB_AddMetaFunc(SLB::Operator<SLB_Declaration_Type>::defaultMult, "__mult")
#define SLB_DefaultDiv()  \
	SLB_AddMetaFunc(SLB::Operator<SLB_Declaration_Type>::defaultDiv, "__div")

#define SLB_Constructor(...) \
	SLB_Constructor_N(SLB_ClassName , __VA_ARGS__ )

#define SLB_Constructor_N(Name, ...) \
	.setConstructor( SLB::FuncCall::create( SLB::Constructor< SLB_Declaration_Type(__VA_ARGS__)>::create ) , Name)

#define SLB_GlobalFunc(Function) SLB_GlobalFunc_N(Function, #Function)

#define SLB_GlobalFunc_N(Function, Name) \
    	SLB::Manager::get().set( SLB::FuncCall::create(Function), Name); 

#define SLB_Value(Value) SLB_Value_N(Value, #Value)

#define SLB_Value_N(Value, Name) \
    	SLB::Manager::get().setValue( Value, Name); 

#endif
