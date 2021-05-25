cmake_minimum_required(VERSION 3.0.0)
project(glib_tutorial VERSION 0.1.0)

# the `pkg_check_modules` function is created with this call
find_package(PkgConfig REQUIRED)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
    pkg_check_modules(GOBJECT REQUIRED IMPORTED_TARGET gobject-2.0)
    pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)
    
    link_libraries(${GLIB_LIBRARIES} ${GOBJECT_LIBRARIES} ${GIO_LIBRARIES})
    link_directories(${GLIB_LIBRARY_DIRS} ${GOBJECT_LIBRARY_DIRS} ${GIO_LIBRARY_DIRS})
    include_directories(${GLIB_INCLUDE_DIRS} ${GOBJECT_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
else()
    include_directories(
        /usr/include/glib-2.0 
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
    )

    link_libraries(
        glib-2.0
        gobject-2.0
    )
endif()

add_subdirectory(hashtable)
add_subdirectory(gio)
# add_subdirectory(thread)
add_subdirectory(eventloop)
# add_subdirectory(gobject-tutorial)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
