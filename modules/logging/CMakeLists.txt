cmake_minimum_required(VERSION 3.27)
project(LifeRhythmLog)

set(INCLUDES
        include
        include/model
        include/view
        include/view/pp_newtype
        include/view/pp_startlogging
        include/view/wg_a
)

set(INCLUDE_FILES
        include/ModuleLog.h
        include/model/Period.h
        include/model/PeriodType.h
        include/model/RecordType.h
        include/view/pp_newtype/PeriodTypeColorBox.h
        include/view/pp_startlogging/PeriodTypeIcon.h
        include/view/pp_startlogging/PeriodTypeTreeWidget.h
        include/view/pp_startlogging/RecordTypeListWidget.h
        include/view/pp_startlogging/WidgetStartPeriodMenu.h
        include/view/wg_a/WidgetA.h
        include/view/wg_a/TimeBar.h
)

set(SRC_FILES
        src/ModuleLog.cpp
        src/model/Period.cpp
        src/model/PeriodType.cpp
        src/model/RecordType.cpp
        src/view/pp_newtype/PeriodTypeColorBox.cpp
        src/view/pp_startlogging/PeriodTypeIcon.cpp
        src/view/pp_startlogging/PeriodTypeTreeWidget.cpp
        src/view/pp_startlogging/RecordTypeListWidget.cpp
        src/view/pp_startlogging/WidgetStartPeriodMenu.cpp
        src/view/wg_a/WidgetA.cpp
        src/view/wg_a/TimeBar.cpp
)

add_library(${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_FILES})
add_library(lr::log ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        lr::core
)