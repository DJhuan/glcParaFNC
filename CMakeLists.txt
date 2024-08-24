cmake_minimum_required(VERSION 3.30)
project(glcParaFNC)

set(SOURCE_FILES main.cpp Glc.hpp Glc.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})