cmake_minimum_required(VERSION 3.7)
project(prog)

set(PKG_NAMES )
set(CXXFLAGS -Wall -Wextra -Wpedantic -ggdb)
set(BOOST_PKGS )

file(GLOB_RECURSE SRC_LIST src/*.cxx)
list(REMOVE_ITEM SRC_LIST
    ${CMAKE_SOURCE_DIR}/src/client.cxx
    ${CMAKE_SOURCE_DIR}/src/server.cxx)
file(GLOB_RECURSE MDLS_LIST src/*.cxxm)

if (DEFINED PKG_NAMES)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(PKGS REQUIRED $ENV{PKG_NAMES} ${PKG_NAMES})
  include_directories(${PKGS_INCLUDE_DIRS})
  list(APPEND LIBS_LIST ${PKGS_LIBRARIES})
endif ()

if (DEFINED BOOST_PKGS)
  find_package(Boost REQUIRED ${BOOST_PKGS})
  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND LIBS_LIST ${Boost_LIBRARIES})
endif ()

set(CMAKE_CXX_STANDARD 11)
add_compile_options($ENV{CXXFLAGS} ${CXXFLAGS})

include_directories(src)

add_executable(server src/server.cxx ${SRC_LIST})
target_link_libraries(server ${LIBS_LIST})

add_executable(client src/client.cxx ${SRC_LIST})
target_link_libraries(client ${LIBS_LIST})
