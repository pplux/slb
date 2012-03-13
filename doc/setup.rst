*****************
Setting things up
*****************

Getting SLB
===========

There are three ways you can get SLB:

1. Regular releases: These are your typical zip file with some code, some
   documentation, and some examples and tests. Just go to the website and get it
   from the downloads section.
2. "Amalgamation" releases: These are releases in which the code has been
   amalgamated into three files, one big ``SLB.hpp`` include, one big
   ``SLB.cpp`` source file, and one big ``lua.c`` file with Lua sources.
3. Mercurial repository: If you feel like living on the bleeding edge, get the
   code from the development repo using mercurial::

      hg clone https://code.google.com/p/slb/

In all cases an up-to-date copy of the Lua sources is included, so you are ready
to go. Of course, if you don't want to use the supplied Lua (maybe you have
installed Lua system-wide using your package manager, or you are already
using Lua in your project in some other way, or whatever), you don't have to.

Setup
=====

To begin integrating SLB into your project, you have again to decide between
several options:

1. Compile SLB right into your project.
2. Compile SLB as a static library.
3. Compile SLB as a dynamic library.


Compile SLB right into your project
-----------------------------------

In order to compile SLB into your project, just add all the ``*.cpp`` files and
``lua.c`` from the ``src/`` directory of the SLB source distribution to your
project's list of source files and the ``include/`` path to your project's
include path. ``lua.c`` #includes all the other Lua source files, so you don't
need to mess with the ``src/lua`` directory.

If you don't want to use the bundled Lua distribution, don't build ``lua.c`` and
set the preprocessor variable ``SLB_EXTERNAL_LUA`` to 1
(``-DSLB_EXTERNAL_LUA=1``).


Compile SLB as a static/dynamic library
---------------------------------------

SLB uses `premake4 <http://industriousone.com/premake>`_ to generate build
scripts for many platforms. This way it can be compiled under a great number of
different platforms. If you want to build SLB as a library, it's highly
recommended you get a premake4 binary. Then you just have to run::

  premake4 <action>

to generate a set of project files or build scripts for your preferred build
system. This project files contain definitions for two targets, SLB-static and
SLB-dynamic, for building (surprise!) static and dynamic libraries.For example,
running::

  premake4 vs2010

would generate a Visual Studio 2010 solution, with the two aforementioned
projects.

After your library is compiled, just add the ``include/`` directory to your
project's include path, and the library to your project's "libraries-to-link"
settings.


