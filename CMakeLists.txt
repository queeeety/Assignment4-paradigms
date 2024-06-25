cmake_minimum_required(VERSION 3.28)
project(Assignment4)

set(CMAKE_CXX_STANDARD 17)

link_directories(/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/libcaesar)
add_executable(Assignment4 main.cpp
        caesar.cpp)
