# How to use Multiple SLB::Manager instances to isolate classes registration #

# Introduction #

Manager instances hold info about the wrapped classes. To access C++ classes from lua scripts you must register the manager first into the lua State (see SLB::Manager::registerSLB). Since SLB version 2.0 you can instantiate any number of Managers and choose which classes are wrapped in each Manager.

The use of multiple managers allows the user to improve the access restriction to the wrapped classes.

The default manager is accessible through the static method SLB::Manager::defaultManager().

# Details #

For example, this how we register a class using the default Manager:

```
SLB::Class< Test >("ExampleNamespace::Test")
    .constructor()
    .set("set", &Test::set)
    .set("get", &Test::get)
;
```

We can also choose to register the class in our own manager:

```
SLB::Manager m;

SLB::Class< Test >("ExampleNamespace::Test", &m)
    .constructor()
    .set("set", &Test::set)
    .set("get", &Test::get)
;
```

Then we must register a manager into a lua\_Script (or use the SLB::Script class) in order to access C++ wrapped classes:

```
SLB::Manager m;
// -- classes registration --
SLB::Script s1(); //< will use the default manager
SLB::Script s2(&m); //< will use the manager "m"

lua_State *L1 = ...;
lua_State *L2 = ...;

SLB::Manager::defaultManager()->registerSLB(L1); //< default manager
m.registerSLB(L2); //< our manager
```


