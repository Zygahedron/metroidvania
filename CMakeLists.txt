cmake_minimum_required(VERSION 4.1)
project(metroidvania VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(HEADER_DIR "${PROJECT_SOURCE_DIR}/include")
set(HEADER_LIST
)
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(SOURCE_LIST
  "${SOURCE_DIR}/main.cpp"
)
source_group(
  TREE "${PROJECT_SOURCE_DIR}/src"
  PREFIX "Source Files"
  FILES ${SOURCE_LIST}
)

find_package(SFML 3.0.2 COMPONENTS Audio Graphics Window System REQUIRED)

add_executable(metroidvania ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(metroidvania PRIVATE ${HEADER_DIR})
target_link_libraries(metroidvania SFML::Audio SFML::Graphics SFML::Window SFML::System)

target_compile_features(metroidvania PRIVATE cxx_std_23)
