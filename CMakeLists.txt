cmake_minimum_required(VERSION 3.10)

project(Strangler-Things)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Console-Game-Engine)
add_subdirectory(Strangler-Things)

file(COPY ${PROJECT_SOURCE_DIR}/Strangler-Things/Resources DESTINATION ${PROJECT_BINARY_DIR}/Strangler-Things/)

exec_program(${PROJECT_BINARY_DIR}/Strangler-Things/Resources/ResourcePackager.exe ${PROJECT_BINARY_DIR}/Strangler-Things/Resources/)