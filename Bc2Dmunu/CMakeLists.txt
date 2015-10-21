# Set the analysis name
SET( ANALYSIS_NAME
  Bc2Dmunu
)

# Define executables
file( GLOB exec_sources "${PROJECT_SOURCE_DIR}/${ANALYSIS_NAME}/main/*.cpp" )
foreach( exec_source ${exec_sources} )
  get_filename_component( source_name ${exec_source} NAME )
  string( REPLACE ".cpp" "" exec_name ${source_name} )
  list(APPEND ANALYSIS_EXECUTABLES ${exec_name})
endforeach()

######################################
#
# set directory names and includes
#
######################################

SET(ANALYSIS_INCLUDE_DIR ./include)
SET(ANALYSIS_SOURCE_DIR ./src)
SET(ANALYSIS_MAIN_DIR ./main)

# include directories
INCLUDE_DIRECTORIES(
	BEFORE
	${ROOT_INCLUDE_DIR}
	${CORE_INC_DIR}
	${ANALYSIS_INCLUDE_DIR}
)

######################################
#
# build the library
#
######################################

AUX_SOURCE_DIRECTORY( ${ANALYSIS_SOURCE_DIR} ANALYSIS_LIB_SOURCES )
IF(HAS_CUSTOMROOTOBJECTS)
	# add the ROOT dictionary to the list of library sources
	SET(ANALYSIS_LIB_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${ANALYSIS_NAME}Dict.cxx ${ANALYSIS_LIB_SOURCES})
ENDIF()
add_library( ${ANALYSIS_NAME}Components SHARED ${ANALYSIS_LIB_SOURCES} )
#ADD_SHARED_LIBRARY( ${ANALYSIS_NAME}Components ${ANALYSIS_LIB_SOURCES} )
IF(HAS_CUSTOMROOTOBJECTS)
	# add the ROOT dictionary to the list of dependencies
	add_dependencies( ${ANALYSIS_NAME}Components ${ANALYSIS_NAME}Dict )
ENDIF()
target_link_libraries( ${ANALYSIS_NAME}Components ${ROOT_LIBRARIES} ${PROJECT_NAME}Components )

######################################
#
# build the execuables
#
######################################

SET(ANALYSIS_LIBS
	${ANALYSIS_NAME}Components
	${PROJECT_NAME}Components
)
foreach( exec ${ANALYSIS_EXECUTABLES} )
	add_executable( ${exec} ${ANALYSIS_MAIN_DIR}/${exec}.cpp )
	target_link_libraries( ${exec} ${ANALYSIS_LIBS} )
endforeach()

######################################
#
# install the binaries from the build directory back into the project subdirectory
#
######################################

foreach( exec ${ANALYSIS_EXECUTABLES} )
  INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exec}
    DESTINATION ${PROJECT_SOURCE_DIR}/${ANALYSIS_NAME}/bin/
	)
endforeach()

######################################
#
# create gammacombo output directories
#
######################################

#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/root         )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots        )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/C      )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/cl     )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/dot    )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/eps    )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/par    )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/pdf    )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/png    )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/root   )
#install(DIRECTORY DESTINATION ${ANALYSIS_NAME}/plots/scanner)

