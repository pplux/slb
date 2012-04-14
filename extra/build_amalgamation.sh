#!/bin/bash
# Amalgamation creator for SLB
#
#   SLB - Simple Lua Binder
#   Copyright (C) 2007-2011 Jose L. Hidalgo Valiño (PpluX)

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#       
#       Jose L. Hidalgo (www.pplux.com)
#       pplux@pplux.com
#

strip_cpp() {
  filename=$1
  shift 1
  gcc -fpreprocessed -dD -E $* | 
    sed -e '/^$/d' \
        -e '/# *include *<SLB/d' \
        -e '/# [0-9][0-9]*/d' \
        >> $filename
}

strip_hpp() {
  filename=$1
  shift 1
   tail -q -n +26 $* | 
    sed -e 's/\(# *include *".*.hpp"\)/\/\/->\1/' \
        -e '/# [0-9][0-9]*/d' \
        >> $filename
}

rm -rf output
mkdir  output
cp -R ../include/SLB/lua output
echo "//$(hg log -l 1 |head -n1)" > output/SLB.cpp
echo "//$(hg log -l 1 |head -n1)" > output/SLB.hpp
cat License.txt >>output/SLB.cpp
cat License.txt >>output/SLB.hpp
echo '#include "SLB.hpp"' >> output/SLB.cpp
strip_cpp output/SLB.cpp ../src/*.cpp
strip_hpp output/SLB.hpp \
  ../include/SLB/Config.hpp\
  ../include/SLB/SPP.hpp\
  ../include/SLB/Export.hpp\
  ../include/SLB/Debug.hpp\
  ../include/SLB/lua.hpp\
  ../include/SLB/Allocator.hpp\
  ../include/SLB/String.hpp\
  ../include/SLB/TypeInfoWrapper.hpp\
  ../include/SLB/Object.hpp\
  ../include/SLB/PushGet.hpp\
  ../include/SLB/Value.hpp\
  ../include/SLB/ref_ptr.hpp\
  ../include/SLB/Table.hpp\
  ../include/SLB/Conversor.hpp\
  ../include/SLB/Error.hpp\
  ../include/SLB/Iterator.hpp\
  ../include/SLB/LuaCall.hpp\
  ../include/SLB/Manager.hpp\
  ../include/SLB/Mutex.hpp\
  ../include/SLB/Property.hpp\
  ../include/SLB/FuncCall.hpp\
  ../include/SLB/Instance.hpp\
  ../include/SLB/ClassInfo.hpp\
  ../include/SLB/Private_FuncCall.hpp\
  ../include/SLB/FuncCall_inline.hpp\
  ../include/SLB/Type.hpp\
  ../include/SLB/Hybrid.hpp\
  ../include/SLB/Class.hpp\
  ../include/SLB/ClassHelpers.hpp\
  ../include/SLB/Script.hpp\
  ../include/SLB/StatefulHybrid.hpp\

# prepare lua file
#echo -e '#define LUA_CORE\n#define luaall_c\n#include "lua/luaconf.h"\n' > output/lua.c
awk '/^#include *"..*"/ {
	    name =substr($2,2,length($2)-2);
      system("cat ../src/" name);
      };
    !/^#include *"..*"/ {print}' < ../src/lua.c  |
    sed -e 's/\(# *include *".*.h"\)/\/\/->\1/' >> output/lua.c

