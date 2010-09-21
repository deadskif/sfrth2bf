<<<<<<< HEAD
cmake_minimum_required(VERSION 2.8)
=======
cmake_minimum_required(VERSION 2.6)
>>>>>>> 771ab7d249a245f338d89edb9a0328f119fbd705
add_executable(sbf2c bf2c.c)

if(ENABLE_BF2C)
install(
	TARGETS sbf2c
	DESTINATION bin
	PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
endif(ENABLE_BF2C)
