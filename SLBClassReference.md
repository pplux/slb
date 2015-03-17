# Introduction #

SLB:Class allows you to register a class into a SLB::Manager before it can be used in lua scripts.

# Reference #

```
SLB::Class< ClassType >("[Namespace::Namespace::]ClassName", Manager)
   .set("name", value)
   .set("name", &ClassType::method)
   .set("name", c_function)
   .set("name", &ParentClass::method)
   .constructor()
   .constructor<T1, T2,..>()
   .dynamic_inherits<T>()
   .static_inhertis<T>()
   .inherits<T>()
   .convertibleTo<T>()
   .property("name", &ClassType::member)
   .const_set("name", &ClassType::const_method)
   .nonconst_set("name", &ClassType::non_const_method)
   .enumValue("name", EnumValue)
   .comment("string")
   .param("string")
   .hybrid()

/* advanced: */
   .class__index( &ClassType::method )
   .class__index( c_function )
   .class__index( lua_c_function )
   .class__newindex( &ClassType::method )
   .class__newindex( c_function )
   .class__newindex( lua_c_function )
   .object__index( &ClassType::method )
   .object__index( c_function )
   .object__index( lua_c_function )
   .object__newindex( &ClassType::method )
   .object__newindex( c_function )
   .object__newindex( lua_c_function )
   .__eq( c_function )
   .iterator("name", begin_method, end_method)
   .const_iterator("name", begin_method, end_method)
   .customIterator<IteratorTraits>("name", begin_geneartor, end_generator)

```

## `.set("name", value)` ##
Adds a new member to the class that will be accessible through instances or the class name. The argument name can have double colons (::) to make the value appear under a certain namespace. Values can be:
  * any C value, the value will be copied (numbers, floats, enums, ...)
  * a **c\_function** a function that will be called when accessing the given member. If the function is called with the colon signature `instance:function(arg1,arg2)` the function gets called with the first argument being the instance over its called, remember that in lua the previous code is syntactic sugar for `instance.function(instance, arg1, arg2)`.
  * a **method**, to bind a C++ method use the & before the function name `&ClassInfo::method`. SLB doesn't support method overloading you must give a different name for each method. To resolve the ambiguity you can either choose to write the full form of the method, or use the full template of .set method:
```
.set("method_name", ( int (ClassType::*)(float,float,const char*) ) &ClassType::method_name)
.set<ClassType, int, float, float, const char*>("method_name", &ClassType::method_name)
```
  * a **method** of ClassType's parent class. This is not necessary as SLB supports inheritance, but if a method is called very often this can speed things up because there is no need to search in base classes for methods given a name.