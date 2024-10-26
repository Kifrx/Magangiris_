cmake_minimum_required(VERSION 3.10)

project(OpenCV_Basic)

find_package(OpenCV REQUIRED)
# Menambahkan executable dari file main.cpp
add_executable(posbola posbola.cpp)
add_executable(bola bola.cpp)

target_link_libraries(posbola ${OpenCV_LIBS})  
target_link_libraries(bola ${OpenCV_LIBS})  