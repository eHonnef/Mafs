# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and
# the comments should hopefully make most of it clear.
#
# For many purposes you don't need to change this file.
# But I marked with <Change> flag where you can tweak the project.

cmake_minimum_required(VERSION 3.14)

# <Change>
# Set project name, version and laguages here.
# Version numbers are available by including "Config.h" in
# the source. See Config.h.in for some more details.
project(
  Mafs            # Project name
  VERSION 1.0     # Project version
  LANGUAGES CXX   # Project language
)

SET(CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS ON)
SET(CMAKE_CXX_STANDARD 20)

# Check if this is the main file or a subproject
set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MAIN_PROJECT ON)
endif()

# <Change>
# Options: Things you can set via commandline options to cmake (e.g. -DENABLE_LTO=[ON|OFF])
option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code." ON)
option(ENABLE_THREADS "Enable multithreading" OFF)

# <Change> Is this a single header lib?
# If ON, then you can remove the src/ folder
set(SINGLE_HEADER ON)

# Not main project, disable doctests
if (NOT MAIN_PROJECT)
  set(ENABLE_DOCTESTS OFF)
endif()

# Include stuff.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(FetchContent)
include(ConfigSafeGuards)
include(Colors)
include(Doctest)
include(Documentation)
include(LTO)
include(Misc)
include(Warnings)

# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
#                         Locate files <Change>
# --------------------------------------------------------------------------------
# All .cpp files in src/
# if (NOT SINGLE_HEADER)
#   set(SOURCES
#     src/Matrix/MatrixBase.cpp
#     src/Matrix/Matrix.cpp
#   )
# endif()
# You can change for a recursive way with glob.
# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
# file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
# --------------------------------------------------------------------------------
#                            Build!
# --------------------------------------------------------------------------------
# Compile all sources into a library.
if (SINGLE_HEADER)
  add_library(${PROJECT_NAME} INTERFACE)
elseif (MAIN_PROJECT)
  add_library(${PROJECT_NAME} OBJECT ${HEADERS} ${SOURCES})
else()
  add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
endif()

if (SINGLE_HEADER)
  target_include_directories(
    ${PROJECT_NAME} INTERFACE
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  )
else()
  target_include_directories(
    ${PROJECT_NAME} PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  )
endif()

# --------------------------------------------------------------------------------
#                            External dependencies
# --------------------------------------------------------------------------------
# <Change> the cmake/FetchExternal.cmake file
# Including all external deps
include(FetchExternal)

# Linking external deps
if (SINGLE_HEADER)
  target_link_libraries(${PROJECT_NAME} INTERFACE ${lst_external})
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE ${lst_external})
endif()

# --------------------------------------------------------------------------------
#                            If this is the main project
# --------------------------------------------------------------------------------
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
if (MAIN_PROJECT)

  # There's also (probably) doctests within the library, so we need to see this as well.
  if(ENABLE_DOCTESTS)
    if (SINGLE_HEADER)
      target_link_libraries(${PROJECT_NAME} INTERFACE doctest)
    else()
      target_link_libraries(${PROJECT_NAME} PUBLIC doctest)
    endif()

    add_subdirectory(tests)
  endif()

  # Enable pthread
  if(ENABLE_THREADS)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
  endif()

  # Set the compile options you want.
  # <Change> the cmake/Warnings.cmake file if you want to add/remove/enable/disable some warnings.
  target_set_warnings(${LIBRARY_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying)

  # Add an executable for the main files app/main.cpp.
  # If you add more executables, copy these lines accordingly.
  add_executable(main app/main.cpp)   # Name of exec. and location of file.
  # Link the executable to library (if it uses it).
  # I added the lst_external here to be available in the main file (you can remove)
  target_link_libraries(main PRIVATE ${PROJECT_NAME} ${lst_external})
  target_set_warnings(main ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
  target_enable_lto(main optimized)  # enable link-time-optimization if available for non-debug configurations
  # Insert here the other main file
  list(APPEND target_files main)
endif()

# --------------------------------------------------------------------------------
#                            Project properties
# --------------------------------------------------------------------------------
list(APPEND target_files ${PROJECT_NAME})

# Set the properties you require, e.g. what C++ standard to use. Here applied to library and main (change as needed).
# set_target_properties(
#   ${target_files}
#     PROPERTIES
#       CXX_STANDARD 20
#       CXX_STANDARD_REQUIRED ON
#       CXX_EXTENSIONS ON
#       CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20"
# )
