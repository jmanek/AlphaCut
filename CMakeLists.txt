CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(AlphaChut)

SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive -frounding-math -pthread -fPIC")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
SET(CMAKE_LIBRARY_PATH /opt/local/lib ${CMAKE_LIBRARY_PATH})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/thirdparty/tinyobjloader/
    ${PROJECT_SOURCE_DIR}/thirdparty/CImg/
    ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include/
    ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include/spdlog
)

SET(src
    ${PROJECT_SOURCE_DIR}/src/AlphaCut.cpp
    ${PROJECT_SOURCE_DIR}/src/Barycentric.cpp
    ${PROJECT_SOURCE_DIR}/src/CGAL_Helper.cpp
    ${PROJECT_SOURCE_DIR}/src/Color.cpp
    ${PROJECT_SOURCE_DIR}/src/Export.cpp
    ${PROJECT_SOURCE_DIR}/src/Face.cpp
    ${PROJECT_SOURCE_DIR}/src/HE_Edge.cpp
    ${PROJECT_SOURCE_DIR}/src/HE_Face.cpp
    ${PROJECT_SOURCE_DIR}/src/HE_Vertex.cpp
    ${PROJECT_SOURCE_DIR}/src/Image.cpp
    ${PROJECT_SOURCE_DIR}/src/Load.cpp
    ${PROJECT_SOURCE_DIR}/src/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Material.cpp
    ${PROJECT_SOURCE_DIR}/src/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/TextureCoord.cpp
    ${PROJECT_SOURCE_DIR}/src/Vector2.cpp
    ${PROJECT_SOURCE_DIR}/src/Vertex.cpp)

SET(inc
    ${PROJECT_SOURCE_DIR}/include/AlphaCut.h
    ${PROJECT_SOURCE_DIR}/include/Barycentric.h
    ${PROJECT_SOURCE_DIR}/include/CGAL_Helper.h
    ${PROJECT_SOURCE_DIR}/include/Color.h
    ${PROJECT_SOURCE_DIR}/include/Export.h
    ${PROJECT_SOURCE_DIR}/include/Face.h
    ${PROJECT_SOURCE_DIR}/include/HE_Edge.h
    ${PROJECT_SOURCE_DIR}/include/HE_Face.h
    ${PROJECT_SOURCE_DIR}/include/HE_Vertex.h
    ${PROJECT_SOURCE_DIR}/include/Image.h
    ${PROJECT_SOURCE_DIR}/include/Load.h
    ${PROJECT_SOURCE_DIR}/include/Log.h
    ${PROJECT_SOURCE_DIR}/include/Material.h
    ${PROJECT_SOURCE_DIR}/include/Mesh.h
    ${PROJECT_SOURCE_DIR}/include/TextureCoord.h
    ${PROJECT_SOURCE_DIR}/include/Vector2.h
    ${PROJECT_SOURCE_DIR}/include/Vertex.h)

# CGAL
FIND_PACKAGE(CGAL COMPONENTS Core)
INCLUDE(${CGAL_USE_FILE})

# Boost
FIND_PACKAGE(Boost COMPONENTS system thread)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# X11
FIND_PACKAGE(X11)
#INCLUDE(${X11_INCLUDE_DIR})

# libpng
FIND_PACKAGE(PNG)

ADD_EXECUTABLE(AlphaChut ${src} ${inc})
TARGET_LINK_LIBRARIES(AlphaChut 
                      ${Boost_LIBRARIES}
                      ${X11_LIBRARIES}
                      ${PNG_LIBRARIES})
