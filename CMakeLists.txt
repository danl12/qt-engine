cmake_minimum_required(VERSION 3.16)
project(qt_engine)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Qt5_DIR C:\\Qt\\5.15.0\\msvc2019_64\\lib\\cmake\\Qt5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(qt_engine
        camera.cpp
        camera.h
        entity.cpp
        entity.h
        main.cpp
        material.cpp
        material.h
        mesh.cpp
        mesh.h
        openglwidget.cpp
        openglwidget.h Physics.cpp Physics.h)

include_directories(includes)

find_library(Assimp assimp-vc142-mt lib)
find_library(Bullet3Common
        Bullet3Common
        lib)
find_library(BulletCollision
        BulletCollision
        lib)
find_library(BulletDynamics
        BulletDynamics
        lib)
find_library(BulletInverseDynamics
        BulletInverseDynamics
        lib)
find_library(BulletSoftBody
        BulletSoftBody
        lib)
find_library(LinearMath
        LinearMath
        lib)

target_link_libraries(qt_engine LINK_PUBLIC ${Assimp})
target_link_libraries(qt_engine PRIVATE Qt5::Widgets ${Bullet3Common} ${BulletCollision} ${BulletDynamics} ${BulletInverseDynamics} ${BulletSoftBody} ${LinearMath})

add_custom_command(TARGET qt_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dll"
        $<TARGET_FILE_DIR:qt_engine>)