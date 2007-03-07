import os
import sys
import glob
import SCons.Util

env = Environment()

if os.environ.has_key('CC'):
	env['CC'] = os.environ['CC']
if os.environ.has_key('CFLAGS'):
	env['CCFLAGS'] += SCons.Util.CLVar(os.environ['CFLAGS'])
if os.environ.has_key('CXX'):
	env['CXX'] = os.environ['CXX']
if os.environ.has_key('CXXFLAGS'):
	env['CXXFLAGS'] += SCons.Util.CLVar(os.environ['CXXFLAGS'])
if os.environ.has_key('LDFLAGS'):
	env['LINKFLAGS'] += SCons.Util.CLVar(os.environ['LDFLAGS'])


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
	LINKFLAGS = ['-Wl,-E'],
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
