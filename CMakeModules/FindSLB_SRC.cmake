# - Finds SLB source code 
# and header files to build SLB inside a project.
# To help find SLB, use the variable SLB_SRC_ROOT properly
# 
# Defines the following variables:
# SLB_SRC_FOUND = found necesary files
# SLB_SRC_FILES = (list of files) source files to compile
# SLB_SRC_INCLUDE_DIRS = (directory) main include directory
# SLB_DEFINITIONS = needed definitions to properly compile SLB
#
# TODO: a SLB_SRC_USE_EXTERNAL_LUA option
# TODO: Define the DEBUG level

SET( SLB_SRC_FOUND false )

FIND_PATH( SLB_SRC_INCLUDE_DIRS SLB/SLB.hpp  PATHS ${SLB_SRC_ROOT}/include )
FIND_PATH( SLB_SRC_DIR SLB.cpp PATHS ${SLB_SRC_ROOT}/src)

IF ( SLB_SRC_INCLUDE_DIRS AND SLB_SRC_DIR )
	MESSAGE( STATUS "SLB_SRC found" )
	SET( SLB_SRC_FOUND true )
	FILE( GLOB SLB_SRC_FILES ${SLB_SRC_DIR}/*.cpp )
	SET( SLB_SRC_FILES ${SLB_SRC_FILES} ${SLB_SRC_DIR}/lua.c )
ENDIF ( SLB_SRC_INCLUDE_DIRS AND SLB_SRC_DIR )


