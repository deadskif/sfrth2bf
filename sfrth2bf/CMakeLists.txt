cmake_minimum_required(VERSION 2.6)

include_directories(include)
add_subdirectory(src)

add_executable( sfrth2bf  main.cpp)

set_target_properties(
	sfrth2bf
	PROPERTIES
	 VERSION ${${PROJECT_NAME}_VERSION}
	 OUTPUT_NAME sfrth2bf
)

target_link_libraries( sfrth2bf libsfrth2bf)

install(
	TARGETS sfrth2bf
	DESTINATION bin
	PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
install(
	FILES dict.4th
	DESTINATION share/sfrth2bf
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
)
