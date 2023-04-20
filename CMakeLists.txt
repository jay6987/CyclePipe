cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)

project(CyclePipe)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ${PROJECT_NAME}
  CyclePipe/Timer.cpp
  CyclePipe/SyncBase.cpp
  CyclePipe/PipeBase.cpp
  CyclePipe/PipeReadTokenBase.cpp
  CyclePipe/PipeWriteTokenBase.cpp
  CyclePipe/PipeDumpLoadImpl.cpp
  UnitTests/SyncBaseTest.cpp
  UnitTests/PipeTest.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
)