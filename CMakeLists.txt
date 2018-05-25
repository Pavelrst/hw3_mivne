cmake_minimum_required(VERSION 3.8)
project(hw3_mivne)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        dflow_calc.c
        dflow_calc.h
        dflow_main.c)

add_executable(hw3_mivne ${SOURCE_FILES})