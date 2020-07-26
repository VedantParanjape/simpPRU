cmake_minimum_required(VERSION 3.4)

project(libprurpmsg VERSION 1.0)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${simppru_SOURCE_DIR}/lib")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
  
set(CMAKE_SYSTEM_PROCESSOR pru)
set(CMAKE_C_COMPILER pru-gcc)
set(CMAKE_CXX_COMPILER pru-g++)

######## SOURCES ########
file(GLOB PROJECT_HEADERS ${simppru_SOURCE_DIR}/include/pru/*.h ${simppru_SOURCE_DIR}/include/pru/am335x/*.h)
file(GLOB PROJECT_SOURCES *.c)
set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES})

######## TARGETS ########
set(library_name_pru0 prurpmsg0-${CMAKE_PROJECT_VERSION})
set(library_name_pru1 prurpmsg1-${CMAKE_PROJECT_VERSION})

add_library(${library_name_pru0} STATIC ${PROJECT_FILES})
set_target_properties(${library_name_pru0} PROPERTIES COMPILE_FLAGS "-mmcu=am335x.pru0")

add_library(${library_name_pru1} STATIC ${PROJECT_FILES})
set_target_properties(${library_name_pru1} PROPERTIES COMPILE_FLAGS "-mmcu=am335x.pru1")

target_include_directories(${library_name_pru0} PUBLIC 
  $<BUILD_INTERFACE:${simppru_SOURCE_DIR}/include/pru>
  $<BUILD_INTERFACE:${simppru_SOURCE_DIR}/include/am335x/>)

target_include_directories(${library_name_pru1} PUBLIC 
  $<BUILD_INTERFACE:${simppru_SOURCE_DIR}/include/pru>
  $<BUILD_INTERFACE:${simppru_SOURCE_DIR}/include/am335x/>)

######## INSTALL ########
set(include_destination "include/simppru-${MY_LIBRARY_VERSION}")
set(lib_destination "lib/")
