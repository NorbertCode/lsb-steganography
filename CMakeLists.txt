cmake_minimum_required(VERSION 3.24)
project(lsb_steganography)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    core_lib
    src/images/image.cpp
    src/images/types/bmpImage.cpp
    src/images/imageLoader.cpp
    src/steganography/steganographer.cpp 
    src/steganography/messenger.cpp
    src/steganalysis/steganalyzer.h
    src/steganalysis/types/chi2analyzer.cpp
)
target_include_directories(core_lib PUBLIC src)

# Executable
add_executable(steg 
    src/main.cpp
    src/cli/argParser.cpp
)
target_link_libraries(steg PRIVATE core_lib)
target_include_directories(steg PRIVATE include)

# GoogleTest
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
    run_tests
    tests/sharedTestData.h
    tests/images/imageLoaderTest.cpp
    tests/images/types/bmpImageTest.cpp
    tests/steganography/steganographerTest.cpp
    tests/steganography/messengerTest.cpp
)

target_link_libraries(run_tests PRIVATE 
    core_lib
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)