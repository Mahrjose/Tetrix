cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME Tetrix)

#-----------------------------------------------------------------#
# ======================== Project Setup ======================== #
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

#-----------------------------------------------------------------#
# ========================= Build Type ========================== #
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

#-----------------------------------------------------------------#
# ================= Include Directories & Headers =============== #
add_executable(${PROJECT_NAME} ${SOURCES})  # Entry Point

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/includes
)

target_sources(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/Application
    # Add more source files here ...
)

#-----------------------------------------------------------------#
# ======================= OpenGL Libraries ====================== #
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

#-----------------------------------------------------------------#
# =========================== Linking =========================== #
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL 
    GLUT::GLUT 
    GLEW::GLEW 
    glfw
)

#-----------------------------------------------------------------#
# ================== Debug and Release Options ================== #
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)    
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)         
endif()

#-----------------------------------------------------------------#
# ======================= Output Directories ==================== #
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Ninja")
endif()

#-----------------------------------------------------------------#
# ======================== Compiler Stuff ======================= #
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
