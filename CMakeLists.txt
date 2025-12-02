cmake_minimum_required(VERSION 3.26)

project(pz-map-generator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(fmt CONFIG REQUIRED)
find_package(cpptrace CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)
find_package(SFML COMPONENTS System Graphics Window CONFIG REQUIRED)

# ---- Library ----
file(GLOB_RECURSE LIB_SOURCES "lib/*.cpp")
add_library(pzlib STATIC ${LIB_SOURCES})
target_include_directories(pzlib PUBLIC lib)
target_link_libraries(pzlib PUBLIC
    fmt::fmt
    cpptrace::cpptrace
    lodepng
    SFML::System
    SFML::Graphics
    SFML::Window
)

# ---- Main executable ----
file(GLOB_RECURSE MAIN_SOURCES "main/*.cpp")
add_executable(${PROJECT_NAME} ${MAIN_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE pzlib)

# ---- Test executable ----
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES})
target_include_directories(pzlib PUBLIC lib)
target_link_libraries(tests PRIVATE pzlib doctest::doctest)
