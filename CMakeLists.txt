cmake_minimum_required(VERSION 3.11..3.27)

cmake_policy(SET CMP0076 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_INIT "-Wall -Wpedantic -Werror -Wconversion -Wsign-conversion -Weffc++")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-DDEBUG -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3")

project("Ray Tracer")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main "")
target_sources(main PUBLIC
    "Ray-Tracing/stdafx.cpp"
    "Ray-Tracing/main.cpp"
    "Ray-Tracing/Components/Camera.cpp"
    "Ray-Tracing/Components/Entity.cpp"
    "Ray-Tracing/Components/Light.cpp"
    "Ray-Tracing/Components/Ray.cpp"
    "Ray-Tracing/Components/Renderer.cpp"
    "Ray-Tracing/Components/Surface.cpp"
)

target_link_libraries(
    main PUBLIC
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    glm::glm
    nlohmann_json::nlohmann_json
)