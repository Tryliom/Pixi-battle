# minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name, version and description
project("Pixi-battle" VERSION 0.1 DESCRIPTION "Animated RPG battle")

# Set C++ version
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all cpp and h files
file(GLOB_RECURSE SRC src/*.cpp include/*.h)

# Add executable
add_executable(${PROJECT_NAME} ${SRC})

# Linking libraries
find_package(PkgConfig REQUIRED)

# DPP
pkg_search_module(dpp REQUIRED dpp)
target_link_libraries(${PROJECT_NAME} dpp)

# Magick++
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=8)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

execute_process(COMMAND pkg-config --cflags --libs GraphicsMagick++
        OUTPUT_VARIABLE GraphicsMagick_LINK_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

pkg_search_module(GraphicsMagick REQUIRED GraphicsMagick)
include_directories(${GraphicsMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GraphicsMagick_LIBRARIES} ${GraphicsMagick_LINK_FLAGS})

# Include libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/magic_enum/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/uuid_v4/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Settings
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -march=native")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)