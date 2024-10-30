cmake_minimum_required(VERSION 3.27)
project(LifeRhythm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Program Files/Qt/5.15.2/mingw81_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)

set(INCLUDES
        include/core
        include/core/ui
        include/core/message
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
        include/util/math
        include/widgets
        include
)

set(INCLUDE_FILES
        include/Config.h
        include/Namespaces.h
        include/core/LifeRhythm.h
        include/core/Module.h
        include/core/Plugin.h
        include/core/TickHandler.h
        include/core/ui/MainFrame.h
        include/core/ui/TabBar.h
        include/core/ui/TitledDialog.h
        include/core/message/MessageHandler.h
        include/resources/RcLoaders.h
        include/resources/RcManagers.h
        include/resources/RcTypes.h
        include/util/MemUtil.h
        include/util/Styles.h
        include/util/WidgetUtil.h
        include/util/FocusManager.h
        include/util/math/OpenRange.h
        include/util/math/Pair.h
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
        include/util/io/IByteReader.h
        include/util/io/IByteWriter.h
        include/util/io/Identifier.h
        include/util/io/IOUtil.h
        include/util/io/RcLoader.h
        include/util/io/ResourceManager.h
        include/util/io/StreamByteReader.h
        include/util/io/StreamByteWriter.h
        include/util/io/StringByteReader.h
        include/util/io/StringInput.h
        include/util/io/IOManager.h
        include/util/models/WidgetData.h
        include/util/models/TreeData.h
        include/util/models/ListData.h
        include/util/models/WidgetDataStorage.h
        include/util/models/NestedListData.h
        include/util/nbt/ArrayData.h
        include/util/nbt/BoolData.h
        include/util/nbt/Data.h
        include/util/nbt/FloatData.h
        include/util/nbt/NBTSerializable.h
        include/util/nbt/IntData.h
        include/util/nbt/LongData.h
        include/util/nbt/NBT.h
        include/util/nbt/NBTUtil.h
        include/util/nbt/StringData.h
        include/util/widgetfactory/StandardWidget.h
        include/util/widgetfactory/WidgetFactory.h
        include/util/concurrent/Task.h
        include/util/concurrent/NBTAsyncGetter.h
        include/widgets/ArrowButton.h
        include/widgets/DragWidget.h
        include/widgets/FocusContainer.h
        include/widgets/ImgButton.h
        include/widgets/ImgLabel.h
        include/widgets/LineEdit.h
        include/widgets/ListWidget.h
        include/widgets/Menu.h
        include/widgets/LineEditOptionsBox.h
        include/widgets/ScrollArea.h
        include/widgets/ScrollBar.h
        include/widgets/TextEdit.h
        include/widgets/TextLabel.h
        include/widgets/TreeWidget.h
        include/widgets/Widget.h
        include/widgets/TextButton.h
        include/widgets/GridLayout.h
        include/widgets/ColorSelector.h
        include/widgets/AbstractOptionsBox.h
        include/widgets/IconTextButton.h
        include/widgets/RangeBar.h
        include/widgets/Calendar.h
        include/widgets/SlotsDrawer.h
        include/widgets/Separator.h
)

set(SRC_FILES
        src/Config.cpp
        src/core/LifeRhythm.cpp
        src/core/Module.cpp
        src/core/TickHandler.cpp
        src/core/ui/MainFrame.cpp
        src/core/ui/TabBar.cpp
        src/core/ui/TitledDialog.cpp
        src/core/message/MessageHandler.cpp
        src/resources/RcLoaders.cpp
        src/resources/RcManagers.cpp
        src/resources/RcTypes.cpp
        src/util/MemUtil.cpp
        src/util/Styles.cpp
        src/util/WidgetUtil.cpp
        src/util/FocusManager.cpp
        src/util/math/OpenRange.cpp
        src/util/math/openrange/Lexer.cpp
        src/util/math/openrange/Lexer.h
        src/util/math/openrange/SyntaxAnalyzer.cpp
        src/util/math/openrange/SyntaxAnalyzer.h
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
        src/util/io/IOManager.cpp
        src/util/models/TreeData.cpp
        src/util/models/ListData.cpp
        src/util/models/WidgetDataStorage.cpp
        src/util/models/NestedListData.cpp
        src/util/nbt/ArrayData.cpp
        src/util/nbt/BoolData.cpp
        src/util/nbt/Data.cpp
        src/util/nbt/FloatData.cpp
        src/util/nbt/IntData.cpp
        src/util/nbt/LongData.cpp
        src/util/nbt/NBT.cpp
        src/util/nbt/NBTUtil.cpp
        src/util/nbt/StringData.cpp
        src/util/nbt/NBTSerializable.cpp
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
        src/util/concurrent/NBTAsyncGetter.cpp
        src/widgets/ArrowButton.cpp
        src/widgets/DragWidget.cpp
        src/widgets/FocusContainer.cpp
        src/widgets/ImgButton.cpp
        src/widgets/ImgLabel.cpp
        src/widgets/LineEdit.cpp
        src/widgets/ListWidget.cpp
        src/widgets/Menu.cpp
        src/widgets/LineEditOptionsBox.cpp
        src/widgets/ScrollArea.cpp
        src/widgets/ScrollBar.cpp
        src/widgets/TextEdit.cpp
        src/widgets/TextLabel.cpp
        src/widgets/TreeWidget.cpp
        src/widgets/Widget.cpp
        src/widgets/TextButton.cpp
        src/widgets/GridLayout.cpp
        src/widgets/ColorSelector.cpp
        src/widgets/AbstractOptionsBox.cpp
        src/widgets/IconTextButton.cpp
        src/widgets/RangeBar.cpp
        src/widgets/Calendar.cpp
        src/widgets/SlotsDrawer.cpp
        src/widgets/Separator.cpp
)

include_directories(${INCLUDES})

file(COPY "assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

if (MAKE_DLL)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${INCLUDE_FILES})
    install(TARGETS ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "LifeRhythm" PREFIX "")
else ()
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_FILES} main.cpp) # logo.rc)
    elseif(CMAKE_BUILD_TYPE MATCHES "Release")
        add_executable(${PROJECT_NAME} WIN32  ${SRC_FILES} ${INCLUDE_FILES} main.cpp)#  logo.rc)
    endif ()
endif ()

if(MAKE_DLL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE EXPORT_LIB)
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_LOCAL_COMPILE)
endif ()

target_link_libraries(${PROJECT_NAME}
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