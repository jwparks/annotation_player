cmake_minimum_required(VERSION 3.14)
project(annotation_player)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/jwpark/Qt/5.14.2/clang_64/lib/cmake")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
#find_package(Qt5MediaBindableInterface REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(annotation_player
        main.cpp
        videoplayer.cpp
        videoplayer.h)

target_link_libraries(annotation_player Qt5::Widgets)
target_link_libraries(annotation_player Qt5::Core)
target_link_libraries(annotation_player Qt5::Multimedia)
target_link_libraries(annotation_player Qt5::Gui)
target_link_libraries(annotation_player Qt5::MultimediaWidgets)
#target_link_libraries(annotation_player Qt5::MediaBindableInterface)