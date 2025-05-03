cmake_minimum_required(VERSION 3.10)

project(MyStaticLib)

add_library(MyStaticLib STATIC)

target_sources(MyStaticLib
  PRIVATE
    src/taskbase.cpp
    src/threadpool.cpp
    src/workerthread.cpp
)

target_include_directories(MyStaticLib
  PUBLIC
    include
)