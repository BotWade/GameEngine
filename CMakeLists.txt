add_library(GameEngine SHARED)

add_subdirectory(src)

target_link_libraries(GameEngine glfw)
target_link_libraries(GameEngine OpenGL::GL)
target_link_libraries(GameEngine ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(GameEngine OpenCL)

target_link_libraries(GameEngine ${FREETYPE_LIBRARIES})
target_include_directories(GameEngine PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_compile_options(GameEngine PUBLIC ${FREETYPE_CGLAGS_OTHER})

target_include_directories(GameEngine PUBLIC ${CMAKE_SOURCE_DIR}/GameEngine/src)