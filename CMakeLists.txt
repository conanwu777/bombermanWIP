cmake_minimum_required(VERSION 3.5)
project(opengl_engine)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES
#        src/game_logic/Bomb.cpp
#        src/game_logic/Board.cpp
#        src/game_logic/BreakWall.cpp
#        src/game_logic/Display.cpp
#        src/game_logic/Fire.cpp
#        src/game_logic/general.hpp
#        src/game_logic/Object.cpp
#        src/game_logic/Player.cpp
#        src/game_logic/Time.cpp
#        src/game_logic/ViewObj.cpp
#        src/game_logic/Wall.cpp
#        src/game_logic/main.cpp
        src/main.cpp
        src/Core.cpp
        src/Camera.cpp
        src/ShaderManager.cpp
        src/TextureManager.cpp
        src/ObjectLoader.cpp
        src/Skybox.cpp
        src/CLWrapper.cpp
        src/Noise.cpp
        src/Utils.cpp
        src/RenderManager.cpp
        src/IRenderedObject.cpp
        src/Axes.cpp
        src/TextRenderManager.cpp
        src/TextObject.cpp
        src/AnimationManager.cpp
        src/Cube.cpp
        src/OBJObject.cpp)

set(HEADERS
#        inc/game_logic/Board.hpp
#        inc/game_logic/Bomb.hpp
#        inc/game_logic/BreakWall.hpp
#        inc/game_logic/Display.hpp
#        inc/game_logic/Fire.hpp
#        inc/game_logic/general.hpp
#        inc/game_logic/Object.hpp
#        inc/game_logic/Player.hpp
#        inc/game_logic/Position.hpp
#        inc/game_logic/Time.hpp
#        inc/game_logic/ViewObj.hpp
#        inc/game_logic/Wall.hpp
        inc/Core.hpp
        inc/Camera.hpp
        inc/ShaderManager.hpp
        inc/Constant.hpp
        inc/TextureManager.hpp
        inc/ObjectLoader.hpp
        inc/Skybox.hpp
        inc/CLWrapper.hpp
        inc/Noise.hpp
        inc/Utils.hpp
        inc/RenderManager.hpp
        inc/IRenderedObject.hpp
        inc/Axes.hpp
        inc/stb_image.h
        inc/Vertex.hpp
        inc/TextRenderManager.hpp
        inc/TextObject.hpp
        inc/AnimationManager.hpp
        inc/Cube.hpp
        inc/OBJObject.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (WIN32)
    include(ExternalProject)

    set(freetype_install_location lib/freetype2)
    ExternalProject_Add(freetype2
            GIT_REPOSITORY git://git.sv.nongnu.org/freetype/freetype2.git
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${freetype_install_location}
            )
    include_directories(${freetype_install_location}/include/)
    link_directories(${freetype_install_location}/lib)
endif()

add_executable(opengl_engine ${SOURCES} ${HEADERS})

if (WIN32)
    add_dependencies(opengl_engine freetype2)
    target_link_libraries(opengl_engine freetype ${FREETYPE_LIBRARIES})
endif()

find_package(OpenCL REQUIRED)

include_directories(${OpenCL_INCLUDE_DIRS})

link_directories(${OpenCL_LIBRARY})

# Adding GLI and GLM header only library
include_directories(lib/)

# Adding GLFW for build
add_subdirectory(lib/GLFW)

if (APPLE)
    target_link_libraries(opengl_engine "-framework OpenGL")
	add_subdirectory(lib/freetype-2.9)
	include_directories(lib/freetype-2.9/include)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(opengl_engine ${OPENGL_LIBRARY})
endif()

target_link_libraries(opengl_engine glfw ${GLFW_LIBRARIES} ${OpenCL_LIBRARY} freetype ${FREETYPE_LIBRARIES})

if (WIN32)
    add_subdirectory(lib/)
    add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
    )
    include_directories(lib/glew-2.1.0/include)
    link_libraries(lib/glew-2.1.0/lib)
    target_link_libraries(opengl_engine GLEW)
endif()
