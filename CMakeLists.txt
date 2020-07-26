cmake_minimum_required(VERSION 3.4)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(simppru VERSION 1.0)
enable_language(C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

find_program(pru_gcc pru-gcc)
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc not found!")
endif()

find_program(pru_gcc pru-gcc-ar)
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc-ar not found!")
endif()

find_program(pru_gcc pru-gcc-ranlib)
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc-ranlib not found!")
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/src/pru")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

######## SOURCES ########
file(GLOB PROJECT_HEADERS include/*.h third-party/cJSON/*.h third-party/vec/*.h third-party/klib/*.h)
file(GLOB PROJECT_SOURCES src/*.c third-party/cJSON/*.c third-party/vec/*.c)
set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES})
    
######## TARGETS ########
bison_target(parser
            src/parser.y
            ${CMAKE_CURRENT_SOURCE_DIR}/parser.c)
flex_target(lexer
            src/lexer.l
            ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} ${PROJECT_FILES} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
target_include_directories(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third-party>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

######## INSTALL ########
set(bin_destination "bin")
