include_guard(GLOBAL)

set(INCLUDE_PREFIX ${CMAKE_SOURCE_DIR}/widgets/include)
set(SRC_PREFIX ${CMAKE_SOURCE_DIR}/widgets/src)

set(INCLUDES_WIDGETS
        ${INCLUDE_PREFIX}
        ${INCLUDE_PREFIX}/factory
        ${INCLUDE_PREFIX}/models
        ${INCLUDE_PREFIX}/old
        ${INCLUDE_PREFIX}/widgets
        ${INCLUDE_PREFIX}/widgets/arrowButton
        ${INCLUDE_PREFIX}/widgets/button
        ${INCLUDE_PREFIX}/widgets/calendar
        ${INCLUDE_PREFIX}/widgets/dropDown
        ${INCLUDE_PREFIX}/widgets/focusContainer
        ${INCLUDE_PREFIX}/widgets/inputTextLabel
        ${INCLUDE_PREFIX}/widgets/jbTextButton
        ${INCLUDE_PREFIX}/widgets/label
        ${INCLUDE_PREFIX}/widgets/listWidget
        ${INCLUDE_PREFIX}/widgets/loadingIcon
        ${INCLUDE_PREFIX}/widgets/menu
        ${INCLUDE_PREFIX}/widgets/miniCalendar
        ${INCLUDE_PREFIX}/widgets/slotsPainter
        ${INCLUDE_PREFIX}/widgets/slotsWidget
        ${INCLUDE_PREFIX}/widgets/tiledListWidget
)

set(SRC_FACTORY
        ${INCLUDE_PREFIX}/factory/StandardWidget.h
        ${INCLUDE_PREFIX}/factory/Widget.h
        ${INCLUDE_PREFIX}/factory/WidgetFactory.h
        ${INCLUDE_PREFIX}/factory/WidgetFactoryStorage.h
        ${SRC_PREFIX}/factory/FactoryInit.cpp
        ${SRC_PREFIX}/factory/FactoryInit.h
        ${SRC_PREFIX}/factory/QssParser.cpp
        ${SRC_PREFIX}/factory/QssParser.h
        ${SRC_PREFIX}/factory/StandardWidget.cpp
        ${SRC_PREFIX}/factory/Widget.cpp
        ${SRC_PREFIX}/factory/WidgetFactory.cpp
        ${SRC_PREFIX}/factory/WidgetFactoryRcLoader.cpp
        ${SRC_PREFIX}/factory/WidgetFactoryRcLoader.h
        ${SRC_PREFIX}/factory/WidgetFactoryStorage.cpp
)

set(SRC_MODELS
        ${INCLUDE_PREFIX}/models/ListData.h
        ${INCLUDE_PREFIX}/models/NestedListData.h
        ${INCLUDE_PREFIX}/models/TreeData.h
        ${INCLUDE_PREFIX}/models/WidgetData.h
        ${INCLUDE_PREFIX}/models/WidgetDataStorage.h
        ${SRC_PREFIX}/models/ListData.cpp
        ${SRC_PREFIX}/models/NestedListData.cpp
        ${SRC_PREFIX}/models/TreeData.cpp
        ${SRC_PREFIX}/models/WidgetDataStorage.cpp
)

set(SRC_ARROW_BUTTON
        ${INCLUDE_PREFIX}/widgets/arrowButton/ArrowButton.h
        ${SRC_PREFIX}/widgets/arrowButton/ArrowButton.cpp
)

set(SRC_BUTTON
        ${INCLUDE_PREFIX}/widgets/button/Button.h
        ${SRC_PREFIX}/widgets/button/Button.cpp
)

set(SRC_CALENDAR
        ${INCLUDE_PREFIX}/widgets/calendar/ButtonSwitchView.h
        ${INCLUDE_PREFIX}/widgets/calendar/Calendar.h
        ${INCLUDE_PREFIX}/widgets/calendar/Common.h
        ${INCLUDE_PREFIX}/widgets/calendar/DropDownRange.h
        ${INCLUDE_PREFIX}/widgets/calendar/MiniCalendarDropDown.h
        ${INCLUDE_PREFIX}/widgets/calendar/SlotsPainterWeekdays.h
        ${INCLUDE_PREFIX}/widgets/calendar/SlotsWidgetCalendar.h
        ${SRC_PREFIX}/widgets/calendar/ButtonSwitchView.cpp
        ${SRC_PREFIX}/widgets/calendar/Calendar.cpp
        ${SRC_PREFIX}/widgets/calendar/DropDownRange.cpp
        ${SRC_PREFIX}/widgets/calendar/MiniCalendarDropDown.cpp
        ${SRC_PREFIX}/widgets/calendar/SlotsPainterWeekdays.cpp
        ${SRC_PREFIX}/widgets/calendar/SlotsWidgetCalendar.cpp
)

set(SRC_DROP_DOWN
        ${INCLUDE_PREFIX}/widgets/dropDown/DropDown.h
        ${SRC_PREFIX}/widgets/dropDown/DropDown.cpp
)

set(SRC_FOCUS_CONTAINER
        ${INCLUDE_PREFIX}/widgets/focusContainer/FocusContainer.h
        ${SRC_PREFIX}/widgets/focusContainer/FocusContainer.cpp
)

set(SRC_INPUT_TEXT_LABEL
        ${INCLUDE_PREFIX}/widgets/inputTextLabel/InputTextLabel.h
        ${SRC_PREFIX}/widgets/inputTextLabel/InputTextLabel.cpp
)

set(SRC_JB_TEXT_BUTTON
        ${INCLUDE_PREFIX}/widgets/jbTextButton/JbTextButton.h
        ${SRC_PREFIX}/widgets/jbTextButton/JbTextButton.cpp
)

set(SRC_LABEL
        ${INCLUDE_PREFIX}/widgets/label/Label.h
        ${SRC_PREFIX}/widgets/label/Label.cpp
)

set(SRC_LIST_WIDGET
        ${INCLUDE_PREFIX}/widgets/listWidget/ListWidget.h
        ${SRC_PREFIX}/widgets/listWidget/ListWidget.cpp
)

set(SRC_LOADING_ICON
        ${INCLUDE_PREFIX}/widgets/loadingIcon/LoadingIcon.h
        ${SRC_PREFIX}/widgets/loadingIcon/LoadingIcon.cpp
)

set(SRC_MENU
        ${INCLUDE_PREFIX}/widgets/menu/Menu.h
        ${SRC_PREFIX}/widgets/menu/Menu.cpp
)

set(SRC_MINI_CALENDAR
        ${INCLUDE_PREFIX}/widgets/miniCalendar/MiniCalendar.h
        ${SRC_PREFIX}/widgets/miniCalendar/MiniCalendar.cpp
)

set(SRC_SLOTS_PAINTER
        ${INCLUDE_PREFIX}/widgets/slotsPainter/SlotsPainter.h
        ${SRC_PREFIX}/widgets/slotsPainter/SlotsPainter.cpp
)

set(SRC_SLOTS_WIDGET
        ${INCLUDE_PREFIX}/widgets/slotsWidget/SlotsWidget.h
        ${SRC_PREFIX}/widgets/slotsWidget/SlotsWidget.cpp
)

set(SRC_TILED_LIST_WIDGET
        ${INCLUDE_PREFIX}/widgets/tiledListWidget/TiledListWidget.h
        ${SRC_PREFIX}/widgets/tiledListWidget/TiledListWidget.cpp
)

set(SRC_OLD
        ${INCLUDE_PREFIX}/old/GridLayout.h
        ${INCLUDE_PREFIX}/old/LineEdit.h
        ${INCLUDE_PREFIX}/old/ScrollArea.h
        ${INCLUDE_PREFIX}/old/ScrollBar.h
        ${INCLUDE_PREFIX}/old/TextEdit.h

        ${SRC_PREFIX}/old/GridLayout.cpp
        ${SRC_PREFIX}/old/LineEdit.cpp
        ${SRC_PREFIX}/old/ScrollArea.cpp
        ${SRC_PREFIX}/old/ScrollBar.cpp
        ${SRC_PREFIX}/old/TextEdit.cpp
)

set(SRC_WIDGETS
        ${SRC_FACTORY}
        ${SRC_MODELS}
        ${SRC_ARROW_BUTTON}
        ${SRC_BUTTON}
        ${SRC_CALENDAR}
        ${SRC_DROP_DOWN}
        ${SRC_FOCUS_CONTAINER}
        ${SRC_INPUT_TEXT_LABEL}
        ${SRC_JB_TEXT_BUTTON}
        ${SRC_LABEL}
        ${SRC_LIST_WIDGET}
        ${SRC_LOADING_ICON}
        ${SRC_MENU}
        ${SRC_MINI_CALENDAR}
        ${SRC_SLOTS_PAINTER}
        ${SRC_SLOTS_WIDGET}
        ${SRC_TILED_LIST_WIDGET}
        ${SRC_OLD}
)