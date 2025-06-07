# CMakeList.txt : CMake project for Ray-Tracer, include source and define
# project specific logic here.
#

include_directories("include" "extern")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

# Add source to this project's executable.
add_executable (Ray-Tracer ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Ray-Tracer PROPERTY CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  if (CMAKE_CXX_COMPILER STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused -Wdouble-promotion -Weffc++")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DDNDEBUG")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()
  endif()
endif()

# TODO: Add tests and install targets if needed.
