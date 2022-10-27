cmake_minimum_required( VERSION 3.7.0 )
project( main )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#Append elements to list with: list(APPEND CMAKE_MODULE_PATH <path>)
#CMAKE_MODULE_PATH contains the list of search paths for CMake modules
#It is used by include() or find_package()
list(APPEND CMAKE_MODULE_PATH ${SRC_DIR}/cmake/sdl2)
#Specify the sources of the project
file(GLOB SOURCES
"${SRC_DIR}/*.h"
"${SRC_DIR}/*.c"
"${SRC_DIR}/*/*.h"
"${SRC_DIR}/*/*.c"
)
#Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})
#Find the packages SDL2 and SDL2_ttf with files *.cmake in
#${SRC_DIR}/cmake/sdl2 of CMAKE_MODULE_PATH
find_package( SDL2 )
find_package( SDL2_ttf )
find_package( SDL2_image )
if( ${SDL2_FOUND} AND ${SDL2_TTF_FOUND} AND ${SDL2_image_FOUND})
message(STATUS "Found SDL2 and SDL2_TTF and SDL2_IMAGE")
# Add a directory to list of include directory for a target
# The keyword PRIVATE indicating that the directory is directly
# added to the target’s include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)
# Link the found libraries with *.cmake into the project
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::TTF SDL2::Image)
else()
message(STATUS "Could not locate SDL2 or SDL2_TTF")
endif()
