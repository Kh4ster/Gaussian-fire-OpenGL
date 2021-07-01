cmake_minimum_required(VERSION 3.17)
project(My2727GL LANGUAGES CXX)

# ---
# Detect build type (release or debug)
# ---

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")

    string(REGEX REPLACE ".*/build_" "" BUILD_DIR_NAME ${CMAKE_BINARY_DIR})
    if(${BUILD_DIR_NAME} STREQUAL "debug")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ---
# Find find_package
# ---

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} $(GLEW_INCLUDE_DIRS) ${GLM_INCLUDE_DIRS})

# ---
# Targets
# ---

SET(SRC src/image_io.cc
	src/image.cc
	src/main.cc)

# Binary file
add_executable(render ${SRC})
target_link_libraries(render ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARY})

# Copy binary to parent folder
add_custom_target(copy_render
    COMMAND ${CMAKE_COMMAND} -E copy render "../"
    DEPENDS render
    COMMENT "Copy `render` executable"
    VERBATIM
)

# Run the copied binary in parent folder as working directory
add_custom_target(run
    COMMAND ./render
    DEPENDS copy_render
    WORKING_DIRECTORY ../
    COMMENT "run`render` executable"
    VERBATIM
)
