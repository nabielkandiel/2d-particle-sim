cmake_minimum_required(VERSION 3.20)

# ---- Project ----
project(elastic2d
  VERSION 0.1.0
  LANGUAGES C CXX
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_ASAN  ON)
option(ENABLE_UBSAN  ON)
option(ENABLE_LTO  ON)

# ---- raylib ----
# Try to find an existing raylib (e.g., via package manager or vcpkg).
# If not found, fetch from GitHub.
set(RAYLIB_VERSION_MIN 5.0)

find_package(raylib ${RAYLIB_VERSION_MIN} QUIET CONFIG)

if(NOT raylib_FOUND)
  message(STATUS "raylib not found. Fetching via FetchContent...")
  include(FetchContent)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
  )
  # Raylib build options (keep examples/tools off)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(raylib)
endif()

# ---- Targets ----
# Adjust the source list to match your files.
# Example layout:
#   include/particle.hpp
#   include/simulation.hpp
#   src/main.cpp
#   src/simulation.cpp
set(SRC
  src/main.cpp
  src/simulation.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
# Link raylib (and platform libs if raylib didn't add them)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# only care about clang/gcc
target_compile_options(elastic2d PRIVATE
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion
    -Wshadow -Wdouble-promotion
    -Wformat=2 -Wundef
    -Wnon-virtual-dtor -Woverloaded-virtual
)

# Sanitizers in Debug
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  if(ENABLE_ASAN)
    target_compile_options(elastic2d PRIVATE -fsanitize=address)
    target_link_options(elastic2d PRIVATE    -fsanitize=address)
  endif()
  if(ENABLE_UBSAN)
    target_compile_options(elastic2d PRIVATE -fsanitize=undefined)
    target_link_options(elastic2d PRIVATE    -fsanitize=undefined)
  endif()
endif()

  # LTO for non-Debug builds
if(ENABLE_LTO AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_ok OUTPUT ipo_err)
  if(ipo_ok)
    set_property(TARGET elastic2d PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "IPO/LTO not supported: ${ipo_err}")
    target_compile_options(elastic2d PRIVATE -flto)
    target_link_options(elastic2d PRIVATE    -flto)
  endif()
endif()



# target_compile_definitions(${PROJECT_NAME} PRIVATE ELASTIC2D_USE_RAYLIB=1)

# ---- Output Directories ----
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)

# ---- Nice messages ----
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Raylib: ${raylib_FOUND}")
