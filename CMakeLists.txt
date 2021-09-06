cmake_minimum_required(VERSION 3.16)

project(GUMBO CXX)
project(GUMBO VERSION 0.0.2)

add_subdirectory(src)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Debug)

LIST(APPEND included_headers 
    "cstdio" "cstdarg" "iostream" "csignal" "cstdlib" "ctime" "cstring" "cerrno" "fcntl.h" "unistd.h" "termios.h" "curses.h" )

foreach(header ${included_headers}) 
   find_path( header PATHS /usr/include)
    if(NOT header)
        message(FATAL_ERROR "Can't find folder containing ${header}")
    else()
        message("-- ${header} -- found")
    endif()
endforeach()

add_compile_options("-g")
add_subdirectory(lib/logger)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Filip Ivanusec")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GUMBO Editor")
set(CPACK_PACKAGE_VENDOR "Filip Ivanusec")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_CONTACT "Filip Ivanusec")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
include(CPack)
