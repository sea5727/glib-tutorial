cmake_minimum_required(VERSION 3.0.0)
project(glib_tutorial VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(
    /usr/include/glib-2.0 
    /usr/lib64/glib-2.0/include)

link_libraries(
    glib-2.0
    gio-2.0
    gobject-2.0
)

add_subdirectory(hashtable)
add_subdirectory(gio)
add_subdirectory(thread)
add_subdirectory(eventloop)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
