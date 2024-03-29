cmake_minimum_required(VERSION 3.27)
project(LifeRhythmLogging)

set(LOGGING_INCLUDES
        include
        include/model
        include/view
        include/view/pp_newtype
        include/view/pp_startlogging
        include/view/tab_main
CACHE FORCE STRING INTERNAL)

set(INCLUDE_FILES
        include/model/Period.h
        include/model/PeriodType.h
        include/model/RecordType.h
        include/view/pp_newtype/PeriodTypeColorBox.h
        include/view/pp_startlogging/PeriodTypeIcon.h
        include/view/pp_startlogging/PeriodTypeTreeWidget.h
        include/view/pp_startlogging/RecordTypeListWidget.h
        include/view/pp_startlogging/WidgetStartPeriodMenu.h
        include/view/tab_main/MainTab.h
        include/view/tab_main/TimeBar.h
)

set(SRC_FILES
        src/model/Period.cpp
        src/model/PeriodType.cpp
        src/model/RecordType.cpp
        src/view/pp_newtype/PeriodTypeColorBox.cpp
        src/view/pp_startlogging/PeriodTypeIcon.cpp
        src/view/pp_startlogging/PeriodTypeTreeWidget.cpp
        src/view/pp_startlogging/RecordTypeListWidget.cpp
        src/view/pp_startlogging/WidgetStartPeriodMenu.cpp
        src/view/tab_main/MainTab.cpp
        src/view/tab_main/TimeBar.cpp
)

set(RESOURCES
        ../../assets/log/factories/factories.qrc
)

add_library(${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_FILES} ${RESOURCES})
add_library(lr::logging ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${LOGGING_INCLUDES}
        ${LifeRhythmCore_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        lr::core
)