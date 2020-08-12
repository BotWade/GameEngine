cmake_minimum_required(VERSION 3.0.0)
project(BattleEngine VERSION 0.1.0)

cmake_policy(SET CMP0072 NEW)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenCL REQUIRED)
find_package(PkgConfig REQUIRED) 

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O0")

include(CTest)
enable_testing()

include_directories(src/Graphics/include)
link_directories(src/Graphics/include)

pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)

add_executable(GameEngine src/Main.cpp)

add_custom_command(TARGET GameEngine PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/Data $<TARGET_FILE_DIR:GameEngine>/Data)

add_subdirectory(src/Collisions)
add_subdirectory(src/Core)
add_subdirectory(src/Graphics)
add_subdirectory(src/InterfaceObject)
add_subdirectory(src/Math)
add_subdirectory(src/Object)
add_subdirectory(src/Mesh)
add_subdirectory(src/OpenCL)
add_subdirectory(src/Mesh/ObjLoader)

target_link_libraries(GameEngine glfw)
target_link_libraries(GameEngine OpenGL::GL)
target_link_libraries(GameEngine ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(GameEngine OpenCL)

target_link_libraries(GameEngine ${FREETYPE_LIBRARIES})
target_include_directories(GameEngine PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_compile_options(GameEngine PUBLIC ${FREETYPE_CGLAGS_OTHER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
