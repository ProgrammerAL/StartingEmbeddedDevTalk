cmake_minimum_required(VERSION 3.11)

#Be careful with the if statement. CMake gets weird with variables
#	This is here to check that Google Test has only been Downloaded and compiled one time even though
#	it's referenced by multiple projects. We do this because those projects should be able to be built separatly
if(NOT DEFINED ENV{GTestCompiledAndSubdirectoried})
	set(ENV{GTestCompiledAndSubdirectoried} TRUE)

	#Download/Compile Google Test to be used by this project
	#	Mostly Copy/Pasted from https://github.com/google/googletest/tree/master/googletest#incorporating-into-an-existing-cmake-project
	set(GTestDownloadDir "${CMAKE_BINARY_DIR}/googletest-download")
	set(GTestSrcDir "${CMAKE_BINARY_DIR}/googletest-src")
	set(GTestBinDir "${CMAKE_BINARY_DIR}/googletest-build")
	
	# Download and unpack googletest at configure time
	configure_file(CMakeLists.txt.in ${GTestDownloadDir}/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${GTestDownloadDir} )
	if(result)
		message(FATAL_ERROR "CMake step for googletest failed: ${result}")
	endif()
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${GTestDownloadDir} )
	if(result)
		message(FATAL_ERROR "Build step for googletest failed: ${result}")
	endif()

	# Prevent overriding the parent project's compiler/linker
	# settings on Windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add googletest directly to our build. This defines
	# the gtest and gtest_main targets.
	add_subdirectory(${GTestSrcDir}
						${GTestBinDir}
						EXCLUDE_FROM_ALL)

endif()