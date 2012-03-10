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
    luaL_dostring(L, "print 'hello from lua'");

Binding your first class
========================

For the purposes of this section, let's consider a simple class like this one:

.. code-block:: c++

    class FirstClass
    {
    public:
        FirstClass() : _string(), _int(0)
        {
            std::cout << "FirstClass constructor "<< (void*) this << std::endl;
        }

        ~FirstClass()
        {
            std::cout << "FirstClass destructor " << (void*) this << std::endl;
        }

        FirstClass(const FirstClass &m) : _string(m._string), _int(m._int)
        {
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
    

Some interesting things to note about this snippet: the use of
``SLB.using(SLB)`` to avoid qualifying each appearance of ``FirstClass`` and the
use of ``SLB.copy()`` to invoke the copy constructor of the C++ class.

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



