cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(1ex)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable( 1ex
                main.cpp
                ListGraph.cpp
                MatrixGraph.cpp
                SetGraph.cpp
                ArcGraph.cpp
                GraphFunction.cpp
)
