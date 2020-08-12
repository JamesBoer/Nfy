
# Require minimum standard version of CMake
cmake_minimum_required (VERSION 3.10)

# Set project name
project(UnitTests)

# Set C++ 14 compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files and dependencies to library
set(
    source_list
	"catch.hpp"
    "Main.cpp"
	"TestNotify.cpp"
	"UnitTest.h"
)
add_executable(${PROJECT_NAME} ${source_list})

# Set warning level
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if(UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(UnitTests Threads::Threads)
endif()	

# Organize IDE folders
foreach(source IN LISTS source_list)
	source_group("Source" FILES "${source}")
endforeach()

