Simple Lua Binder (SLB)
-------------------------

SLB is an "simple-to-use" library to wrap C/C++ code and use it from lua scripts.  SLB can be compiled as dynamic library, or it can be included in your project, see INSTALLATION for further instructions.  

The aim of SLB is to provide an easy to use library to wrap code to/from lua and extend your applications. SLB can also be used as a reflection library for C/C++, you can query all the registered classes and ask for its methods, metadata, and so on.

SLB includes a multiplatform implementation of mutexes to ensure thread safety, it can be enabled or disabled or even replaced with you own implementation. Thread safety in SLB is being tested and should be considered in beta state.

USING SLB
---------

The best way to learn how to use SLB is looking at the examples, they are numbered in order of complexity. SLB uses its own namespace, and every piece of data SLB requires is handled by the SLB::Manager class (which follows the singleton pattern). You don't have to care about SLB's internal classes, just take care on your own classes and how they are handled by SLB. 

The SLB::Script class can be used as a base class to handle lua scripts, but it is not mandatory, you can still use SLB on your own lua_states, for example:

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Script s;
	s.doString(lua_code);

or, if you're creating your own lua_States then:

    // Create a lua State, using normal lua API
    lua_State *L = luaL_newstate();
    // load default functions (the current example uses print)
    // and by default, SLB::Script does this.
    luaL_openlibs(L);
    // Register SLB inside the lua_State, calling the SLB::Manager
    // that handles bindings, default functions, default values...
    SLB::Manager::getInstance().registerSLB(L);
    // Now call lua API to execute the same code as above
    luaL_dostring(L, lua_code); // execute code

Once, SLB is registered inside a lua_State, you can start using your wrapped
classes. 

To wrap a class, you must declare its contents, for example:

	SLB::Class< FirstClass >("Namespace::FirstClass")
	// declares an empty constructor
        .constructor()
        .set("getString", &FirstClass::getString)
        .set("setString", &FirstClass::setString)
        .set("getInt", &FirstClass::getInt)
        .set("setInt", &FirstClass::setInt)
	;

This code must be executed somewhere before a script can make use of the
"FirstClass" class. Classes can declare just one constructor, and overloading
is not supported. The ".set" statements can be used for anything, not just methods of the class, you can use it with values, enums, c functions, and so on.


By default SLB registers all classes in the same manager, the default manager.That allows any script to access all registered classes. If you need to limit a script to access just some set of C++ classes you can create managers and selectively register classes into it. To do so, you pass a pointer to your manager, as second argument of SLB::Class construction:
	
	SLB::Manager MyManager;

	SLB::Class< FirstClass >("Namespace::FirstClass", MyManager)
        .constructor()
	...


SLB From Lua
------------

SLB doesn't pollute the lua global namespace, it stores everything under a table called SLB. Even your classes are there, but you can populate a namespace to the global table callint "SLB.using(namespace)" from lua. For example, without the using the above our class could be instantiated from the script with:

	fc = SLB.Namespace.FirstClass()

With the using, we can do:

	SLB.using(SLB.Namespace)
	fc = FirstClass()

Once we have received/created a class we can access to its methods, but
instead of the dot(.) notation in lua we use the colon(:) for example:

	fc:setString("a string")
	print(fc:getString())

More advanced use of SLB is exposed in the examples and wiki docs (under construction).

INSTALLATION
------------

SLB uses premake, but it is mainly used for the SLB's development. You are encouraged to use SLB as part of your own library/program for the sake of simplicity, but it is ready to be used as a dynamic library.  

To include SLB as part of your own project, grab the src/ and include/ directories and compile all the .cpp/c files under src/. If SLB is going to be part of a dll on Windows , or a dll itself, then enable the macro declaration SLB_LIBRARY that will properly adjust the SLB_EXPORT macro. 

