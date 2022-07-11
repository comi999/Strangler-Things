file(GLOB TEST_PROJECT_SOURCES ./*.cpp)
file(GLOB TEST_PROJECT_HEADERS ./*.hpp)

add_definitions(-DUNICODE)
add_executable(Strangler-Things ${TEST_PROJECT_SOURCES} ${TEST_PROJECT_HEADERS})
target_link_libraries(Strangler-Things PUBLIC CGE)

target_include_directories(Strangler-Things PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Console-Game-Engine/CGE"
    )