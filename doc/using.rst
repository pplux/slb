*********
Using SLB
*********

First steps
===========

The first thing you need to get up and running is a Lua interpreter. If you are
already familiar with Lua you will probably be glad to know you can use all the
usual ``lua_State`` management functions. Nevertheless, a wrapper called
``SLB::Script`` is provided in order to ease things.

Lua bindings are controlled by means of an object of type ``SLB::Manager``. A
``Manager`` controls a ``lua_State`` (which holds the complete state of a Lua
interpreter), and only a ``Manager`` can be associated to any given
``lua_State``. You can begin executing Lua code doing just this::

    SLB::Manager m;
    SLB::Script s(&m);
    s.doString("print 'hello from lua'");

or, if you want to manage your lua_States manually, doing it like this::

    // this code is equivalent to the previous example using SLB::Script
    SLB::Manager m;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    m.registerSLB(L);
    luaL_dostring(L, "print 'Hello from lua!'");

From now on, we will use the first, simpler, approach. Now let's turn it into a
complete program. There is actually little more to do: just include
``SLB/SLB.hpp`` and put the previous chunk of code somewhere useful::

    #include "SLB/SLB.hpp"

    int main() {
        SLB::Manager m;
        SLB::Script s(&m);
        s.doString("print 'hello from lua'");
        return 0;
    }

If you build and run this last snippet, you will see Lua greeting you on the
standard output :). Note how ``SLB::Script::doString()`` takes a C string and
executes it as Lua. This is the simplest way to get SLB running code. For more
complex uses there is also ``SLB::Script::doFile()``, which takes the path of a
Lua source file and runs it, and the "safe" variants of both ``safeDoString()``
and ``safeDoFile()``, which are guaranteed to never throw exceptions or end your
program.


Binding C functions
===================

Now that we can run Lua code, it's time we get started on interaction between
Lua and C. The simplest thing you can probably think in this regard is exposing
a C function to Lua code, and thus it should be easy with SLB. Let's try::

    #include "SLB/SLB.hpp"
    #include <cstdio>

    void hello() {
        printf("Hello from C!\n");
    }

    int main() {
        SLB::Manager m;
        SLB::Script s(&m);
        m.set("c_hello", SLB::FuncCall::create(hello));
        s.doString("SLB.c_hello()");
        return 0;
    }

What we are doing here is creating a ``SLB::FuncCall`` object, which represents
the Lua binding for a C function (in this case ``hello``), and then calling
``SLB::Manager::set()``, which assigns the name ``c_hello`` to the newly created
function. It's important to remember that SLB exports everything to Lua inside a
table called SLB. That is the reason why the Lua code says ``SLB.c_hello()``
instead of simply ``hello()``.

The cool thing about ``SLB::FuncCall`` is that works whatever the signature of
the function you are binding is, as long as you register the types of the
parameters before. Let's see it in action with different built-in types for the
parameters and the return value::

    #include "SLB/SLB.hpp"
    #include <cstdio>

    float example(const char *message, int i) {
        printf("message=%s\n", message);
        return i/2.0f;
    }

    int main() {
        SLB::Manager m;
        SLB::Script s(&m);
        m.set("example", SLB::FuncCall::create(example));
        s.doString("print( SLB.example('this is a message', 9) )");
        return 0;
    }

As you can see, it works as expected :)


Binding C++ classes
===================

Your first class
----------------

So, we have seen how to run simple C functions from Lua, but if you are using
SLB, which is a C++ library, chances are you have many classes in your project,
and you want to access some of them from Lua. SLB provides facilities to expose
your classes to Lua so you can create objects, copy them, call their methods,
and pass them from C to Lua and back.

For the purposes of this section, let's consider a simple class like this one:

.. code-block:: c++

    class FirstClass {
    public:
        FirstClass() : _string(), _int(0) {
            std::cout << "FirstClass constructor "<< (void*) this << std::endl;
        }

        ~FirstClass() {
            std::cout << "FirstClass destructor " << (void*) this << std::endl;
        }

        FirstClass(const FirstClass &m) : _string(m._string), _int(m._int) {
            std::cout << "FirstClass copy constructor from " << (void*) &m << " -> " << (void*) this << std::endl;
        }

        int getInt() const { return _int; }
        const std::string& getString() const { return _string; }
        void setInt(int i) { _int = i; }
        void setString(const std::string &s) { _string = s; }

    private:
        std::string _string;
        int _int;
    };

To access this class from Lua code, first we need to create a binding.
Fortunately, easing the creation of bindings is what SLB is all about. The
templated class ``SLB::Class`` is the tool to use. Let's see it with an
example::

    SLB::Class< FirstClass >("FirstClass",m)
        .constructor() // empty constructor
        .set("getString", &FirstClass::getString)
        .set("setString", &FirstClass::setString)
        .set("getInt", &FirstClass::getInt)
        .set("setInt", &FirstClass::setInt)
    ;

As you can see, a ``SLB::Class`` is created with our class name as the template
parameter, and two constructor parameters: the name our class will have in Lua
code, and a pointer to our ``Manager``. Then the method ``.set()`` is called for
each method we wish to bind.  The first parameter to ``set()`` is the Lua name
of the method, and the second is a pointer to that method. You can also bind
different constructors with ``constructor()`` for the empty (default)
constructor, and with ``constructor<Type1, Type2, ...>()`` for constructors with
parameters.

After the class has been bound, it can be instanced and used in Lua code:

.. code-block:: lua

    print(SLB.FirstClass)
    c1 = SLB.FirstClass()
    SLB.using(SLB) -- sets SLB methods/classes avialable globally
    c2 = FirstClass()
    c1:setString('c1_string')
    c2:setString('c2_string')
    print('c1',c1:getString())
    print('c2',c2:getString())
    print'c3 = c2 will copy a reference, *NO* real copy'
    c3 = c2 
    print'changing c2 will affect c3'
    c2:setString('c2_string_v2')
    print('c2',c2:getString())
    print('c3',c3:getString())
    print'now we copy c2 returning a new instance'
    c3 = SLB.copy(c2) 
    print'now c3 is a new instance independent from futher changes to c2'
    c2:setString('c2_string_v3')
    print('c2',c2:getString())
    print('c3',c3:getString())
    

Some interesting things to note about this snippet:

* C++ objects are created by means of a function with the same name as our
  class (in this case ``FirstClass``, which wraps the C++ constructor.

* Methods from our C++ class are called using the usual colon notation 
  ``object:method()`` for Lua objects.

* ``SLB.using(SLB)`` can be used to avoid qualifying each appearance of
  ``FirstClass``.

* ``SLB.copy()`` is used to invoke the copy constructor of a class.

You can also add documentation strings for the class, for each method, and for
each parameter of each method using ``.comment()`` and ``.param()``. The
previous example with docstrings would look like this::

    SLB::Class< FirstClass >("FirstClass",m)
        .comment("This is our wrapper of FirstClass class!!")
        .constructor() // empty constructor
        .set("getString", &FirstClass::getString)
            .comment("Returns the internal string")
        .set("setString", &FirstClass::setString)
            .comment("sets the internal string")
            .param("the string to copy")
        .set("getInt", &FirstClass::getInt)
            .comment("returns the internal int")
        .set("setInt", &FirstClass::setInt)
            .comment("sets the internal int value")
            .param("the int to copy")
    ;


**TODO**: What can we do with documentation strings?

Dealing with inheritance
------------------------

**TODO**

- .inherits() etc, we can probably put an example without abstract classes here to
  avoid introducing policies so soon (we probably want abstract classes to be
  NoCopy like in 05_funcalls.

- try to show polymorphism in the example

- multiple inheritance?


Binding static methods
----------------------

**TODO**

Just elaborate a bit on the static part in 04_static_and_C example.

Policies
--------

Let's consider now a class like ``FirstClass`` in the previous section, but
without a copy constructor::

    class FirstClass {
    public:
        FirstClass() : _string(), _int(0) {
            std::cout << "FirstClass constructor "<< (void*) this << std::endl;
        }

        ~FirstClass() {
            std::cout << "FirstClass destructor " << (void*) this << std::endl;
        }

        int getInt() const { return _int; }
        const std::string& getString() const { return _string; }
        void setInt(int i) { _int = i; }
        void setString(const std::string &s) { _string = s; }

    private:
        std::string _string;
        int _int;
    };


If we try to bind this class as in the previous example, we will find out that
SLB tries to bind the copy constructor so that ``SLB.copy`` works. Therefore, we
need a way to tell SLB that a class is non-copyable, which we can do passing a
*policy* as a template argument to ``SLB::Class``, like this::


    // same binding as before, but with a NoCopy policy
    SLB::Class< MyClass, SLB::Instance::NoCopy >("MyClass",m)
        .constructor<const std::string&, int>()
        .set("getString", &MyClass::getString)
        .set("setString", &MyClass::setString)
        .set("getInt", &MyClass::getInt)
        .set("setInt", &MyClass::setInt)
    ;


Other possible policies are briefly described in the list below. Please go to
the reference section for a more in-depth description:

``SLB::Instance::NoCopyNoDestroy``
    Objects of the class are non-copyable. SLB doesn't call any destructors.

``SLB::Instance::SmartPtr<T_sm>``
    Typical SmartPointer based object, where  T_sm<T> can be instantiated (like
    auto_ptr).

``SLB::Instance::SmartPtrNoCopy<T_sm>``
    SmartPointer, with disabled copy.

``SLB::Instance::SmartPtrSharedCopy<T_sm>``
    SmartPointer, but the copy is based on the copy of T_sm itself.


Calling Lua from C++
====================



Manipulating Lua Variables
==========================





