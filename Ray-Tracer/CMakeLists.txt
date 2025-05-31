# CMakeList.txt : CMake project for Ray-Tracer, include source and define
# project specific logic here.
#

include_directories("include")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

# Add source to this project's executable.
add_executable (Ray-Tracer ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Ray-Tracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
