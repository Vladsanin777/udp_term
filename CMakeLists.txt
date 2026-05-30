cmake_minimum_required(VERSION 3.16)

project(udp VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_path(UDP_INCLUDE_DIR udp.h PATHS /usr/local/include /usr/include)

find_library(UDP_SHARED_LIB NAMES udp PATHS /usr/local/lib /usr/lib)
find_library(UDP_STATIC_LIB NAMES libudp.a PATHS /usr/local/lib /usr/lib)

set(PROJECT_SOURCES 
    udp.c 
)

add_executable(shared_udp ${PROJECT_SOURCES})

target_include_directories(shared_udp PRIVATE ${UDP_INCLUDE_DIR})

target_link_libraries(shared_udp PRIVATE 
    ${UDP_SHARED_LIB}
)

install(TARGETS shared_udp
    RUNTIME DESTINATION bin
    COMPONENT shared-app
)

add_executable(static_udp ${PROJECT_SOURCES})

target_include_directories(static_udp PRIVATE ${UDP_INCLUDE_DIR})

target_link_libraries(static_udp PRIVATE 
    ${UDP_STATIC_LIB}
)

install(TARGETS static_udp
    RUNTIME DESTINATION bin
    COMPONENT static-app
)
