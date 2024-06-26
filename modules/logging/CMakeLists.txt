cmake_minimum_required(VERSION 3.27)
project(LifeRhythmLog)

set(INCLUDES
        include
        include/model
        include/view
        include/view/pp_newtype
        include/view/pp_startlogging
        include/view/wg_a
        include/view/wg_b
        include/view/wg_b/b1
        include/view/shared
)

set(INCLUDE_FILES
        include/ModuleLog.h
        include/model/Period.h
        include/model/UsageType.h
        include/model/RecordType.h
        include/model/WeekViewData.h
        include/model/DRSData.h
        include/model/UsageInfo.h
        include/model/Usage.h
        include/model/DataA1.h
        include/view/pp_newtype/PeriodTypeColorBox.h
        include/view/pp_startlogging/UsageTypeIcon.h
        include/view/pp_startlogging/UsageTypeTreeWidget.h
        include/view/pp_startlogging/RecordTypeListWidget.h
        include/view/pp_startlogging/WidgetStartPeriodMenu.h
        include/view/wg_a/A.h
        include/view/wg_a/A0.h
        include/view/wg_a/A1.h
        include/view/wg_b/B.h
        include/view/wg_b/B0.h
        include/view/wg_b/B2.h
        include/view/wg_b/B3.h
        include/view/wg_b/B4.h
        include/view/wg_b/b1/B1.h
        include/view/wg_b/b1/DateRangeSelector.h
        include/view/shared/TimeBarItem.h
        include/view/shared/TimeScale.h
        include/view/shared/UsageTypeLabel.h
)

set(SRC_FILES
        src/ModuleLog.cpp
        src/model/Period.cpp
        src/model/UsageType.cpp
        src/model/RecordType.cpp
        src/model/WeekViewData.cpp
        src/model/DRSData.cpp
        src/model/UsageInfo.cpp
        src/model/Usage.cpp
        src/model/DataA1.cpp
        src/view/pp_newtype/PeriodTypeColorBox.cpp
        src/view/pp_startlogging/UsageTypeIcon.cpp
        src/view/pp_startlogging/UsageTypeTreeWidget.cpp
        src/view/pp_startlogging/RecordTypeListWidget.cpp
        src/view/pp_startlogging/WidgetStartPeriodMenu.cpp
        src/view/wg_a/A.cpp
        src/view/wg_a/A1.cpp
        src/view/wg_a/TimeBar.cpp
        src/view/wg_b/B.cpp
        src/view/wg_b/B0.cpp
        src/view/wg_b/B2.cpp
        src/view/wg_b/B3.cpp
        src/view/wg_b/B4.cpp
        src/view/wg_b/b1/B1.cpp
        src/view/wg_b/b1/DateRangeSelector.cpp
        src/view/shared/TimeBarItem.cpp
        src/view/shared/TimeScale.cpp
        src/view/shared/UsageTypeLabel.cpp
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