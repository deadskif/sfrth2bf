cmake_minimum_required(VERSION 2.6)

#set( CMAKE_CXX_COMPILER g++ )
#set( CMAKE_CXX_FLAGS "-Wall -W -O2" )
find_package( FLEX )

flex_target( Scanner frth_lex.l ${CMAKE_CURRENT_BINARY_DIR}/frth_lex.cpp
	COMPILE_FLAGS -+)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
#	-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
#add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
set(libsfrth2bf_SRCS
	compiler.cpp
	config.cpp
	word.cpp
	${FLEX_Scanner_OUTPUTS} )

add_library( libsfrth2bf SHARED ${libsfrth2bf_SRCS})
set_target_properties(
	libsfrth2bf
	PROPERTIES
	 VERSION ${${PROJECT_NAME}_VERSION}
	 SOVERSION ${${PROJECT_NAME}_VERSION}
	 OUTPUT_NAME sfrth2bf
)
install(
	TARGETS libsfrth2bf LIBRARY
	DESTINATION lib
	PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE
		     GROUP_READ GROUP_EXECUTE
		     WORLD_READ WORLD_EXECUTE
)

