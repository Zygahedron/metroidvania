cmake_minimum_required(VERSION 4.1)
project(metroidvania VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
  ${SRCROOT}/main.cpp
)
source_group("" FILES ${SRC})

add_executable(metroidvania ${SRC})

target_include_directories(metroidvania PRIVATE ${INCROOT})

find_package(SFML 3.0 COMPONENTS Audio Graphics Window System REQUIRED)

target_link_libraries(metroidvania sfml-audio sfml-graphics sfml-window sfml-system)

