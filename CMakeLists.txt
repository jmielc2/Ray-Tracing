cmake_minimum_required(VERSION 3.11..3.27)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("Ray Tracer")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)

get_cmake_property("vars" VARIABLES)
foreach("var" ${vars})
    message(${var} = ${${var}})
endforeach()

include_directories("/usr/local/include/SDL2/")
include_directories("/usr/local/include/glm/")
link_directories("/usr/local/lib/")

add_executable(main "")
target_sources(main PRIVATE
    "Ray-Tracing/stdafx.cpp"
    "Ray-Tracing/main.cpp"
    "Ray-Tracing/Components/Camera.cpp"
    "Ray-Tracing/Components/Entity.cpp"
    "Ray-Tracing/Components/Light.cpp"
    "Ray-Tracing/Components/Ray.cpp"
    "Ray-Tracing/Components/Renderer.cpp"
    "Ray-Tracing/Components/Surface.cpp"
)
target_link_libraries(main ${SDL2_LIBRARIES} "SDL2_image")
