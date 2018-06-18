cmake_minimum_required(VERSION 3.8)
project(Seam_Carving)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SeamCarver.cpp SeamCarver.h)
add_executable(Seam_Carving ${SOURCE_FILES})

find_package(OpenCV)
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( Seam_Carving ${OpenCV_LIBS} )