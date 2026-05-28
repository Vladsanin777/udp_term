cmake_minimum_required(VERSION 3.16)

project(udp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

find_path(UDP_INCLUDE_DIR udp.h PATHS /usr/local/include /usr/include)

find_library(UDP_SHARED_LIB NAMES udp PATHS /usr/local/lib /usr/lib)
find_library(UDP_STATIC_LIB NAMES libudp.a PATHS /usr/local/lib /usr/lib)

set(PROJECT_SOURCES 
    udp.cpp 
)

add_executable(shared_udp ${PROJECT_SOURCES})

target_include_directories(shared_udp PRIVATE ${UDP_INCLUDE_DIR})

target_link_libraries(shared_udp PRIVATE 
    Qt6::Core 
    Qt6::Network
    Qt6::Widgets
    ${UDP_SHARED_LIB}
)

install(TARGETS shared_udp
    RUNTIME DESTINATION bin
    COMPONENT shared-app
)

add_executable(static_udp ${PROJECT_SOURCES})

target_include_directories(static_udp PRIVATE ${UDP_INCLUDE_DIR})

target_link_libraries(static_udp PRIVATE 
    Qt6::Core 
    Qt6::Network
    Qt6::Widgets
    ${UDP_STATIC_LIB}
)

install(TARGETS static_udp
    RUNTIME DESTINATION bin
    COMPONENT static-app
)
