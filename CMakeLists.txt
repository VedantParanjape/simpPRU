# Specify minimum cmake version required
cmake_minimum_required(VERSION 3.4)

# Make sure that it is not built in root directory, build inside /build folder
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

# CMake project description and project specific settings
project(simppru 
        VERSION 1.3
        DESCRIPTION "Compiler for simpPRU, compiles simpPRU code down to PRU-C"
        HOMEPAGE_URL "https://github.com/VedantParanjape/simpPRU/")
enable_language(C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Check for required packages
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_program(pru_gcc pru-gcc)

# Check if pru-gcc is installed
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc not found!")
endif()

# Check if pru-gcc-ar is installed
find_program(pru_gcc pru-gcc-ar)
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc-ar not found!")
endif()

# Check if pru-gcc-ranlib is installed
find_program(pru_gcc pru-gcc-ranlib)
if(NOT pru_gcc)
    message(FATAL_ERROR "pru-gcc-ranlib not found!")
endif()

# Add subdirectory with a subproject
add_subdirectory("${CMAKE_SOURCE_DIR}/src/pru")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/console")

# Specify Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

######## SOURCES ########
# Add source files and header files
file(GLOB PROJECT_HEADERS include/*.h third-party/cJSON/*.h third-party/vec/*.h third-party/klib/*.h)
file(GLOB PROJECT_SOURCES src/*.c third-party/cJSON/*.c third-party/vec/*.c)
set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES})
    
######## TARGETS ########
# Add bison and flex targets
bison_target(parser
            src/parser.y
            ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.c)
flex_target(lexer
            src/lexer.l
            ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

# Add executables
add_executable(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} ${PROJECT_FILES} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
target_compile_definitions(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} PRIVATE INSTALL_PATH=${CMAKE_INSTALL_PREFIX} VERSION_NUMBER=${CMAKE_PROJECT_VERSION})
target_include_directories(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third-party>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

######## INSTALL ########
# Install generated binaries
set(bin_destination "bin")
install(TARGETS ${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} DESTINATION ${bin_destination})

# Create symlink to remove version name and add install the symlink
install(CODE "execute_process(COMMAND ln -sf ${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})")
install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} DESTINATION ${bin_destination})

######## Debian Package generator ########
EXECUTE_PROCESS(COMMAND dpkg --print-architecture COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)

if (UNIX)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Vedant Paranjape")
    set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}")
    set(CPACK_PACKAGE_ARCHITECTURE "${ARCHITECTURE}")
    set(CPACK_DEBIAN_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${ARCHITECTURE}")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS)

    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Compiler for simpPRU, compiles simpPRU code down to PRU-C")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "standard")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/VedantParanjape/simpPRU/")
    set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.md")
    set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE__SOURCE_DIR}/script/postinst")

    include(CPack)
endif()
