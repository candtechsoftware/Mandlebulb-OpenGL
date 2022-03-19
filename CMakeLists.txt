cmake_minimum_required(VERSION 3.8)
project(mandlebulb LANGUAGES CXX)
add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/Camera.cpp
  src/Window.cpp
)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # creates compile_command.json

set_property(TARGET ${PROJECT_NAME} PROPERTY CMAKE_CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -g)

add_definitions(-DGLEW_STATIC)
add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glew EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)
add_subdirectory(vendor/imgui-cmake EXCLUDE_FROM_ALL)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
