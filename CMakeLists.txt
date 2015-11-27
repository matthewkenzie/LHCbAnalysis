# require a certain cmake version
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# declare the project name
PROJECT(LHCbAnalysis)

# set project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

# install destinations can be passed via the command line:
IF(NOT DEFINED LIBRARY_OUTPUT_DIRECTORY)
	set( LIBRARY_OUTPUT_DIRECTORY ./lib )
ENDIF()
IF(NOT DEFINED INCLUDE_OUTPUT_DIRECTORY)
	set( INCLUDE_OUTPUT_DIRECTORY ./include )
ENDIF()

# include cmake macros and settings
LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
INCLUDE(analyses)

# use ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# find ROOT
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(ROOT REQUIRED COMPONENTS MathCore MathMore RIO Hist Tree RooFit RooFitCore TMVA RooStats)

# Get Boost
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(Boost_INCLUDE_DIR "/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/include/boost-1_53")
  set(Boost_LIBRARY_DIR "/afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt/lib" )
  set(ENV{Boost_ROOT} /afs/cern.ch/sw/lcg/external/Boost/1.53.0_python2.7/x86_64-slc6-gcc47-opt)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.53 REQUIRED COMPONENTS program_options system filesystem REQUIRED)

# find Doxygen
FIND_PACKAGE(Doxygen)

if(DOXYGEN_FOUND)
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
	# add target
	ADD_CUSTOM_TARGET(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
ELSE()
	MESSAGE( STATUS "Doxygen not installed or found -- API documentation can't be generated!" )
ENDIF(DOXYGEN_FOUND)

######################################
#
# this stuff is not needed?
#
######################################

#IF(NOT CMAKE_BUILD_TYPE)
#  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
#      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
#      FORCE)
#ENDIF(NOT CMAKE_BUILD_TYPE)

## require proper c++, standard is C++03, flag reads: c++98
##SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi -pedantic -Wno-long-long -Wshadow -Werror=overloaded-virtual"   )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -O3 -g3")

######################################
#
# check for C++11
#
######################################

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 HAS_CPP11)
check_cxx_compiler_flag(-std=c++0x HAS_CPP11_ALTERNATIVE)
if (HAS_CPP11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (HAS_CPP11_ALTERNATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message("No C++11 support.  Moving on.")
endif()

# set additional compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-extensions"   )

######################################
#
# set includes
#
######################################

# Set core include directory
SET( CORE_INC_DIR ./core/include/ )
# Set custom roofit class directory
SET( CUSTOM_ROOT_INC_DIR ./roofit/include/ )

# include directories
INCLUDE_DIRECTORIES(
	BEFORE
	${ROOT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CUSTOM_ROOT_INC_DIR}
	${CORE_INC_DIR}
)
# declare the ROOT library path
LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})

# define the sources of the core library
AUX_SOURCE_DIRECTORY( ./core/src          CORE_LIB_SOURCES )
AUX_SOURCE_DIRECTORY( ./roofit/src        CORE_LIB_SOURCES )

######################################
#
# generate ROOT dictionaries
#
######################################

# generate root dictionary
file ( GLOB TMP_CUSTOM_ROOT_INCS roofit/include/Roo*.h )
# strip full path name
foreach( fil ${TMP_CUSTOM_ROOT_INCS} )
  file( RELATIVE_PATH pat ${PROJECT_SOURCE_DIR}/core/include ${fil} )
  list( APPEND CUSTOM_ROOT_INCS ${pat} )
endforeach()
# add to root dict
set( CUSTOM_ROOT_DICT ${PROJECT_NAME}Dict )
set( CUSTOM_ROOT_LINKDEF "LinkDef.h" )
ROOT_GENERATE_DICTIONARY( ${CUSTOM_ROOT_DICT}
  ${CUSTOM_ROOT_INCS}
  LINKDEF ${CUSTOM_ROOT_LINKDEF}
)
add_custom_target( ${CUSTOM_ROOT_DICT} DEPENDS ${CUSTOM_ROOT_DICT}.cxx ${CUSTOM_ROOT_DICT}.h )
list( APPEND CORE_LIB_SOURCES ${CUSTOM_ROOT_DICT}.cxx )

######################################
#
# build core library
#
######################################
add_library( ${PROJECT_NAME}Components SHARED ${CORE_LIB_SOURCES} )
#set_target_properties( ${PROJECT_NAME}Components PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION} )
# add the ROOT dictionary to the list of dependencies
add_dependencies( ${PROJECT_NAME}Components ${CUSTOM_ROOT_DICT} )
#set_property(TARGET ${PROJECT_NAME}Components PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib")
# add ROOT libraries to the linker command
target_link_libraries( ${PROJECT_NAME}Components ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )

######################################
#
# subdirectorys for the combiners
#
######################################

foreach( dir ${ANALYSIS_MODULES} )
	add_subdirectory(${dir})
endforeach()

######################################
#
# tests
#
######################################

## the examples, build with "make tests"
#ADD_CUSTOM_TARGET( tests )
#MACRO( ADD_GENFIT_TEST _testname )
#   ADD_EXECUTABLE( ${_testname} EXCLUDE_FROM_ALL ${ARGN} )
#   ADD_DEPENDENCIES( tests  ${_testname} )
#   TARGET_LINK_LIBRARIES( ${_testname} ${PROJECT_NAME}  ${ROOT_LIBS} -lGeom )
#    #INSTALL( TARGETS ${_testname} DESTINATION ${EXECUTABLE_INSTALL_DIR})
#ENDMACRO( ADD_GENFIT_TEST )
#
#ADD_GENFIT_TEST( fitterTests               test/fitterTests/main.cc)
#ADD_GENFIT_TEST( minimalFittingExample     test/minimalFittingExample/main.cc)
#ADD_GENFIT_TEST( measurementFactoryExample test/measurementFactoryExample/main.cc)
#ADD_GENFIT_TEST( streamerTest              test/streamerTest/main.cc)
#ADD_GENFIT_TEST( unitTests                 test/unitTests/main.cc)
#if(DEFINED RAVE)
#  ADD_GENFIT_TEST( vertexingTest           test/vertexingTest/main.cc)
#  ADD_GENFIT_TEST( vertexingTestRead       test/vertexingTest/read.cc)
#endif()

# generate and install following configuration files
#GENERATE_PACKAGE_CONFIGURATION_FILES( genfitConfig.cmake genfitConfigVersion.cmake )


