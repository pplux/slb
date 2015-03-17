#SLB3-Policies controls how C++ objects interacts with lua

# Introduction #

Every object wrapped into lua must use a policy that tells SLB how the object must be handled. There are four different scenarios handled by policies:


  * Instance Creation: A new object is created inside lua

  * Pointer is passed to lua: A pointer is given to the policy, what to do with that pointer is up to the policy in use. For example: copy the object, keep a reference, use a ref-counting mechanism...

  * A const-pointer is passed to lua: similar to a pointer case, but now the object must be treated as immutable.

  * Pointer unpack: When C++ code requires a pointer from an object inside lua (that got there following one of the three previous options)

Internally references are treated as pointers, you don't have to care about them. SLB can be extended by implementing custom policies when required. There are three standard policies implemented ready to be used:

  * [default](https://code.google.com/p/slb/source/browse/include/SLB3/policy/default.h): objects created inside lua are released by lua, pointers or const-pointers given are stored as pointers, and SLB will not release them.

  * [nodelete](https://code.google.com/p/slb/source/browse/include/SLB3/policy/nodelete.h): Objects can't be created inside lua, and it will only accept pointers/const-pointers, the memory will never be released by lua.

  * [smartptr](https://code.google.com/p/slb/source/browse/include/SLB3/policy/smartptr.h): This is a templated policy that can be use with some common smart pointers. Here what is actually stored inside lua is the smart pointer handler.

# Detail of the smartptr policy #

Currently the most "complex" policy implemented is the [smart-pointer](https://code.google.com/p/slb/source/browse/include/SLB3/policy/smartptr.h) policy, we are going to use as reference in case you need to modify it, or create your own policy.

## New Instance Creation ##

In order to allow SLB create new instances of objects inside lua the instance-creation bits must be implemented. It is optional to implement as long as you don't try to compile code that will try to create new instances (for example, constructor, or returning an object by copy)

The process of creating new instances is split in two steps:

  * `static Object NewInstanceBegin(lua_State *L, void **raw_mem)`
  * `static void NewInstanceEnd(const Object obj, T* new_obj)`

The first method is called to before the instance is created to prealloc all the needed memory, the second is called once the object is created and initialized just in case you want to do something with it. The later can be empty, but must be declared.

```
  static Object NewInstanceBegin(lua_State *L, void **raw_mem) {
    Object obj = PushRawObject<T>(L, kObject_Instance, sizeof(SmartT));
    *raw_mem = AllocatorT::alloc(); // alloc call
    return obj;
  }
```

The Method `PushRawObject` is implemented in `common.h` header, it basically creates an SLB::Object that internally handles C++ objects inside lua.

For Smart pointers we need to allocate space for the smart pointer itself, that is what SLB::Object will represent
```
    Object obj = PushRawObject<T>(L, kObject_Instance, sizeof(SmartT));
```


Also, we must allocate the memory required to build a new Instance:
```
    *raw_mem = AllocatorT::alloc(); // alloc call
```
SLB will use that memory to build the object using the [placement new](http://en.wikipedia.org/wiki/Placement_syntax) and the wrapped constructor.

After the execution of `NewInstanceBegin` no instance is created, only the memory required is allocated. Once the object is built, SLB will call to `NewInstanceEnd`:

```
 static void NewInstanceEnd(const Object obj, T* new_obj) {
    assert(obj.header->type == kObject_Instance && "Invalid instance");
    new (obj.data) SmartT(new_obj);
  }
```

Here we have the opportunity to build the smart pointer using the placement new with the memory that previously we've allocated. Object-T's constructor has been called and we receive a pointer to the new created instance.

Default policy is simpler, for comparison here it is:
```
  static Object NewInstanceBegin(lua_State *L, void **raw_mem) {
    Object obj = PushRawObject<T>(L, kObject_Instance, sizeof(T));
    *raw_mem = obj.data;
    return obj;
  }

  static void NewInstanceEnd(const Object obj, T* /*obj*/) {
    assert(obj.header->type == kObject_Instance && "Invalid instance");
    /* Do nothing */
  }
```

As you can see, the main difference is that the `PushRawObject` will actually allocate memory for the object itself, no handler is needed. Also `NewInstanceEnd` is empty.

## Passing [const](const.md) Pointers (or References) ##
```
  static void PushPtr(lua_State *L, T* src) {
    Object obj = PushRawObject<T>(L, kObject_Ptr, sizeof(SmartT));
    new (obj.data) SmartT(src);
  }

  static void PushConstPtr(lua_State *L, const T* src) {
    Object obj = PushRawObject<T>(L, kObject_Const_Ptr, sizeof(const T*));
    *reinterpret_cast<const T**>(obj.data) = src;
  }
```

Here we can see a big difference in handling const, from non const pointer, using smart-pointers. In the first case we create an Smart pointer, in the second case we only store the pointer to the object.

References are treated by SLB as pointers, you don't need to worry about that.

## Pointer unpack ##

The process of extracting a pointer from the SLB::Object falls into the function unpack:

```
  static void* Unpack(const Object &obj) {
    switch(obj.header->type) {
      case kObject_Invalid: assert(!"Invalid object"); return NULL;
      case kObject_Ptr:
      case kObject_Instance:
        return reinterpret_cast<SmartT*>(obj.data)->get();
      case kObject_Const_Ptr:
        return *reinterpret_cast<T**>(obj.data);
    }
    assert(!"Invalid policy::Struct::Unpack call");
    return NULL;
  }
```

What to do on each case depends on the policy itself.


## Destructor ##
Once lua collects our object we may need to execute a piece of code in order to destroy the Instance, for example:
```
  static void Destructor(const Object obj) {
    if(obj.header->type == kObject_Instance || obj.header->type == kObject_Ptr) {
      SmartT *smart_ptr = reinterpret_cast<SmartT*>(obj.data);
      smart_ptr->~SmartT();
    }
  }
```

Here we test if the object is a Instance-type, or a pointer that was passed, and in that case we manually call the destructor of the smart pointer class. No need to free the memory, that will be done by lua.