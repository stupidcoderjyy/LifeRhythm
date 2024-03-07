cmake_minimum_required(VERSION 3.27)
project(LifeRhythm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)

set(INCLUDES
        include/core/ui
        include/core
        include/resources
        include/util
        include/util/compile
        include/util/error
        include/util/highlight
        include/util/io
        include/util/nbt
        include/util/models
        include/util/widgetfactory
        include/util/concurrent
        include/widgets
        include
)

set(INCLUDE_FILES
        include/Config.h
        include/Namespaces.h
        include/core/LifeRhythm.h
        include/core/Plugin.h
        include/core/ui/MainFrame.h
        include/core/ui/TabBar.h
        include/core/ui/TitledDialog.h
        include/resources/RcLoaders.h
        include/resources/RcManagers.h
        include/resources/RcTypes.h
        include/util/MemUtil.h
        include/util/Styles.h
        include/util/WidgetUtil.h
        include/util/compile/CompileError.h
        include/util/compile/Compiler.h
        include/util/compile/CompilerInput.h
        include/util/error/Error.h
        include/util/error/ErrorHandler.h
        include/util/error/PrintErrorHandler.h
        include/util/highlight/Highlighter.h
        include/util/highlight/StyleParser.h
        include/util/io/AbstractInput.h
        include/util/io/BufferedInput.h
        include/util/io/DataLoader.h
        include/util/io/IByteReader.h
        include/util/io/IByteWriter.h
        include/util/io/Identifier.h
        include/util/io/IOUtil.h
        include/util/io/RcLoader.h
        include/util/io/ResourceManager.h
        include/util/io/Serializable.h
        include/util/io/StreamByteReader.h
        include/util/io/StreamByteWriter.h
        include/util/io/StringByteReader.h
        include/util/io/StringInput.h
        include/util/models/WidgetData.h
        include/util/models/TreeData.h
        include/util/models/ListData.h
        include/util/models/SelectableListData.h
        include/util/nbt/ArrayData.h
        include/util/nbt/BoolData.h
        include/util/nbt/Data.h
        include/util/nbt/FloatData.h
        include/util/nbt/IntData.h
        include/util/nbt/NBT.h
        include/util/nbt/NBTUtil.h
        include/util/nbt/StringData.h
        include/util/widgetfactory/StandardWidget.h
        include/util/widgetfactory/WidgetFactory.h
        include/util/concurrent/Task.h
        include/widgets/ArrowButton.h
        include/widgets/DragWidget.h
        include/widgets/FocusContainer.h
        include/widgets/ImgButton.h
        include/widgets/ImgLabel.h
        include/widgets/LineEdit.h
        include/widgets/ListWidget.h
        include/widgets/Menu.h
        include/widgets/OptionsBox.h
        include/widgets/RangeBar.h
        include/widgets/ScrollArea.h
        include/widgets/ScrollBar.h
        include/widgets/SelectableListWidget.h
        include/widgets/TextEdit.h
        include/widgets/TextLabel.h
        include/widgets/TreeWidget.h
        include/widgets/Widget.h
)

set(SRC_FILES
        src/Config.cpp
        src/core/LifeRhythm.cpp
        src/core/Plugin.cpp
        src/core/ui/MainFrame.cpp
        src/core/ui/TabBar.cpp
        src/core/ui/TitledDialog.cpp
        src/resources/RcLoaders.cpp
        src/resources/RcManagers.cpp
        src/resources/RcTypes.cpp
        src/util/MemUtil.cpp
        src/util/Styles.cpp
        src/util/WidgetUtil.cpp
        src/util/compile/CompileError.cpp
        src/util/compile/Compiler.cpp
        src/util/compile/CompilerInput.cpp
        src/util/error/Error.cpp
        src/util/error/PrintErrorHandler.cpp
        src/util/highlight/Highlighter.cpp
        src/util/highlight/StyleParser.cpp
        src/util/io/AbstractInput.cpp
        src/util/io/BufferedInput.cpp
        src/util/io/Identifier.cpp
        src/util/io/IOUtil.cpp
        src/util/io/ResourceManager.cpp
        src/util/io/StreamByteReader.cpp
        src/util/io/StreamByteWriter.cpp
        src/util/io/StringByteReader.cpp
        src/util/io/StringInput.cpp
        src/util/io/DataLoader.cpp
        src/util/models/WidgetData.cpp
        src/util/models/TreeData.cpp
        src/util/models/ListData.cpp
        src/util/models/SelectableListData.cpp
        src/util/nbt/ArrayData.cpp
        src/util/nbt/BoolData.cpp
        src/util/nbt/Data.cpp
        src/util/nbt/FloatData.cpp
        src/util/nbt/IntData.cpp
        src/util/nbt/NBT.cpp
        src/util/nbt/NBTUtil.cpp
        src/util/nbt/StringData.cpp
        src/util/nbt/highlight/NBTHighlighter.cpp
        src/util/nbt/highlight/NBTHighlighter.h
        src/util/nbt/parser/Lexer.cpp
        src/util/nbt/parser/Lexer.h
        src/util/nbt/parser/Parser.cpp
        src/util/nbt/parser/Parser.h
        src/util/widgetfactory/FactoryInit.cpp
        src/util/widgetfactory/QssParser.cpp
        src/util/widgetfactory/StandardWidget.cpp
        src/util/widgetfactory/WidgetFactory.cpp
        src/util/widgetfactory/FactoryInit.h
        src/util/widgetfactory/QssParser.h
        src/util/concurrent/Task.cpp
        src/widgets/ArrowButton.cpp
        src/widgets/DragWidget.cpp
        src/widgets/FocusContainer.cpp
        src/widgets/ImgButton.cpp
        src/widgets/ImgLabel.cpp
        src/widgets/LineEdit.cpp
        src/widgets/ListWidget.cpp
        src/widgets/Menu.cpp
        src/widgets/OptionsBox.cpp
        src/widgets/RangeBar.cpp
        src/widgets/ScrollArea.cpp
        src/widgets/ScrollBar.cpp
        src/widgets/SelectableListWidget.cpp
        src/widgets/TextEdit.cpp
        src/widgets/TextLabel.cpp
        src/widgets/TreeWidget.cpp
        src/widgets/Widget.cpp
)

set(RESOURCES
        assets/lr/factories/factories.qrc
        assets/lr/highlightstyles/styles.qrc
        assets/lr/images/images.qrc
        assets/test/factories/factories.qrc
        assets/log/factories/factories.qrc
)

set(TEST_SRC
        test/main_tab/MainTab.cpp
        test/main_tab/MainTab.h
        test/main_tab/TimeBar.cpp
        test/main_tab/TimeBar.h
        test/main_tab/PeriodWidget.cpp
        test/main_tab/PeriodWidget.h
        test/main_tab/Period.cpp
        test/main_tab/Period.h
        test/main_tab/PeriodType.cpp
        test/main_tab/PeriodType.h
        test/popup_start_record/WidgetStartPeriodMenu.h
        test/popup_start_record/WidgetStartPeriodMenu.cpp
        test/popup_start_record/RecordTypeListWidget.cpp
        test/popup_start_record/RecordTypeListWidget.h
        test/popup_start_record/RecordType.cpp
        test/popup_start_record/RecordType.h
)

include_directories(${INCLUDES} test test/main_tab test/popup_start_record)

set(ALL_FILES ${SRC_FILES} ${INCLUDE_FILES} ${RESOURCES})

if(MAKE_DLL)
    add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
else ()
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        message("Debug Build")
        add_executable(${PROJECT_NAME} ${ALL_FILES} ${TEST_SRC} main.cpp) # logo.rc)
    elseif(CMAKE_BUILD_TYPE MATCHES "Release")
        message("Release Build")
        add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES} ${TEST_SRC} main.cpp)#  logo.rc)
    endif ()
endif ()

target_link_libraries(LifeRhythm
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
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
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
