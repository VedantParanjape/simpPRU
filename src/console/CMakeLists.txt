# Specify minimum cmake version required
cmake_minimum_required(VERSION 3.4)

# Download ftxui library
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Find Threads Library
find_package( Threads )

# CMake project description and project specific settings
project(simppru-console
        VERSION ${CMAKE_PROJECT_VERSION}
        DESCRIPTION "Console for simpPRU"
        HOMEPAGE_URL "https://github.com/VedantParanjape/simpPRU/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set pru-gcc as the compiler

# Specify Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
  
######## SOURCES ########
# Add source files and header files
file(GLOB PROJECT_HEADERS ${simppru_SOURCE_DIR}/include/console/*.hpp)
file(GLOB PROJECT_SOURCES *.cpp)
set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES})

######## TARGETS ########
# Add executables
add_executable(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} ${PROJECT_FILES})
set_target_properties(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} PROPERTIES CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} PUBLIC 
  $<BUILD_INTERFACE:${simppru_SOURCE_DIR}/include/console>
  $<BUILD_INTERFACE:${ftxui_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

# Link ftxui and Thread library
target_link_libraries(${PROJECT_NAME}-${CMAKE_PROJECT_VERSION}
PRIVATE ftxui::screen
PRIVATE ftxui::dom
PRIVATE ftxui::component
${CMAKE_THREAD_LIBS_INIT}
)


######## INSTALL ########
# Install generated binaries
set(bin_destination "bin")
install(TARGETS ${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} DESTINATION ${bin_destination})

# Create symlink to remove version name and add install the symlink
install(CODE "execute_process(COMMAND ln -sf ${PROJECT_NAME}-${CMAKE_PROJECT_VERSION} ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})")
install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} DESTINATION ${bin_destination})
