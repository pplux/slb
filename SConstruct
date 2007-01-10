import os
import sys
import glob

env = Environment()

env.Append(CPPPATH = [
		'#/include',
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

debug = int(ARGUMENTS.get('debug',10))
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
	LIBS = ['SLB','lua','dl']
	)

env.StaticLibrary(
		target = '#/lib/SLB',
		source = glob.glob('src/*.cpp')
		)
