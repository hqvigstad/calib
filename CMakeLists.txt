project(calib)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# ROOT 
find_package(ROOT REQUIRED)
# message(STATUS "root includes: ${ROOT_INCLUDE_DIR}")
# message(STATUS "root libraries: ${ROOT_LIBRARIES}")
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
set(LIBS ${LIBS} ${ROOT_LIBRARIES})


# Build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Woverloaded-virtual -Weffc++ -Wctor-dtor-privacy")


add_subdirectory(calibrators)
add_subdirectory(misc)
add_subdirectory(test)
