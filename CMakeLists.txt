cmake_minimum_required(VERSION 2.8)

project(SHADERTOY)

find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

find_package(Qt4)
set(QT_USE_QTXML 1)
set(QT_USE_QTOPENGL 1)
include(${QT_USE_FILE})

add_executable(main  main.cpp GLSLShader.cpp texture.cpp)
target_link_libraries(main ${GLUT_LIBRARIES} GLEW ${OPENGL_LIBRARIES} ${QT_LIBRARIES})

# add a symbolic link toward the shaders in the source directory
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images
)
