cmake_minimum_required(VERSION 3.27)
project(widgets)
# 禁用详细输出
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.6.3/mingw_64/lib/cmake")
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/util
)
include(Files-widgets)
include(Files-widgets-test)
include(Files-util)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)

if (MAKE_DLL)
    add_library(${PROJECT_NAME} SHARED ${SRC_WIDGETS})
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "lr_${PROJECT_NAME}" PREFIX "")
    target_compile_definitions(${PROJECT_NAME} PRIVATE EXPORT_LIB)
else ()
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_executable(${PROJECT_NAME} ${SRC_WIDGETS} ${TEST_FILE}) # logo.rc)
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
        add_executable(${PROJECT_NAME} WIN32 ${SRC_WIDGETS} ${TEST_FILE})#  logo.rc)
    endif ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_LOCAL_COMPILE)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DLL_PATH "${CMAKE_SOURCE_DIR}/cmake-build-makedll-debug/util/lr_util.dll")
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    set(DLL_PATH "${CMAKE_SOURCE_DIR}/cmake-build-makedll-release/util/lr_util.dll")
endif ()
if (EXISTS ${DLL_PATH})
    file(COPY ${DLL_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
    message(WARNING "缺少DLL:${DLL_PATH}")
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES_WIDGETS} ${INCLUDES_UTIL})

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Concurrent
        lr_util
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()