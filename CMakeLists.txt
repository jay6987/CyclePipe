cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)

project(CyclePipe)

enable_testing()

add_library(
  ${PROJECT_NAME}
  CyclePipe/Timer.cpp
  CyclePipe/SyncBase.cpp
  CyclePipe/PipeBase.cpp
  CyclePipe/PipeReadTokenBase.cpp
  CyclePipe/PipeWriteTokenBase.cpp
  CyclePipe/PipeDumpLoadImpl.cpp
)

add_subdirectory(UnitTests)