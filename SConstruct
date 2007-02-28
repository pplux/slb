import os
import sys
import glob

env = Environment()

env.Append(CPPPATH = [
		'#/include',
		'#/include/lua',
		])

env.Append(LIBPATH = [
		'#/lib',
		])

env.Append(CCFLAGS = [
		'-W',
		'-Wall',
		])

env.Append(LINKFLAGS = [
		])

debug = int(ARGUMENTS.get('debug',0))
if debug > 0:	
	print "********************************"
	print "* Building in DEBUG mode       *"
	print "********************************"
	env.Append(CCFLAGS = ['-g'])
	env.Append(CPPDEFINES = ['SLB_DEBUG_LEVEL='+str(debug)])
else:
	print "********************************"
	print "* Building in RELEASE mode     *"
	print "********************************"
	env.Append(CCFLAGS = ['-O3'])


env.Program(
	target = '#/tests/SLB', 
	source = glob.glob('tests/src/*.cpp'),
	LIBS = ['SLB','dl']
	)
source_files = glob.glob('src/*.cpp') + glob.glob('src/lua/*.c')

env.SharedLibrary(
		target = '#/lib/SLB',
		source = source_files
		)
env.StaticLibrary(
		target = '#/lib/SLB',
		source = source_files
		)
