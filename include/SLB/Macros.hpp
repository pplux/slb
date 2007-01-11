#ifndef __SLB_MACROS__
#define __SLB_MACROS__

// Creates a SLB::FuncCall
#define SLB_Function(FUNC)   SLB::FuncCall::create(FUNC) 

// Starts a new class Definition
#define SLB_CLASS(Class) SLB_CLASS_N(#Class, Class)

// Starts a new class Definition where the "Name" of the bindec
// class is different from the class itself. For examle this is useful
// to bind the class MyTemplate<Instance> with a valid name :)
#define SLB_CLASS_N(S_Name, T_Class)                                               \
		namespace {                                                                  \
		static struct SPP_LINEID(Class_Declaration) {                                \
			const char *SLB_ClassName;                                               \
			SLB::Class *SLB_Class; \
			SPP_LINEID(Class_Declaration)() {                                        \
				typedef T_Class SLB_Declaration_Type;                                  \
				SLB_ClassName = S_Name;                                                \
				SLB_Class = SLB::Manager::getInstance().getOrCreateClass(typeid(SLB_Declaration_Type));\
				SLB_Class->setName(SLB_ClassName); \

// Ends definition
#define SLB_END                     \
		; } /* end class */         \
		} SPP_LINEID(__Dummy__) ; }

// set a value
#define SLB_Set(Name, Value) SLB_Class->set(Name, Value);

// Defines the constructor
#define SLB_Constructor(...) \
	SLB_Class->setConstructor(SLB::Constructor<SLB_Declaration_Type(__VA_ARGS__)>::create());

// Defines a function
#define SLB_Func(Name, F) SLB_Set(Name, SLB_Function(F))

// Defines a method
#define SLB_Method(F) SLB_Method_N(#F, F)
#define SLB_Method_N(Name, FUNC) SLB_Func(Name, &SLB_Declaration_Type::FUNC)


#endif
