#
# Created By: Javier Castorena
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.20.0)

project(eraser VERSION 0.0.1)

set(DEBUG_FLAGS "-Wall -Wextra -Wpedantic -lcurl -dynamic ")
set(RELEASE_FLAGS "-O3 ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(JSON-GLIB REQUIRED json-glib-1.0)
#PKG_CHECK_MODULES(GIO REQUIRED gio-2.0 )
pkg_check_modules(BLKID REQUIRED blkid)
pkg_check_modules(UDEV REQUIRED libudev)
#pkg_check_modules(VTE REQUIRED vte-2.91)
#pkg_check_modules(LIBHW REQUIRED libhw)
#pkg_check_modules(GTOP REQUIRED libgtop-2.0)
#pkg_check_modules(SSH REQUIRED libssh>=0.8.9)

add_executable(${PROJECT_NAME} 
     main.c
    #common.c
    erase_log.c
    build_device.c
    sg_io_linux.c
    sg_cmds.c
    sg_pt_linux.c
    sg_lib.c 
    gui.c
    leak_memory.c
    config.c
    erase_device.c
)

include_directories(
    include
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
    ${JSON-GLIB_INCLUDE_DIRS}
    ${BLKID_INCLUDE_DIRS}
    #${GIO_INCLUDE_DIRS}
    ${UDEV_INCLUDE_DIRS}
    #${VTE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${JSON-GLIB_LIBRARIES}
    ${GLIB_LIBRARIES}
#    ${CMAKE_THREAD_LIBS_INIT}
    ${BLKID_LIBRARIES}
    ${UDEV_LIBRARIES}
    ${GTK3_LIBRARIES}
#    ${VTE_LIBRARIES}
#    ${GIO_LIBRARIES}
)