include_guard(GLOBAL)

set(INCLUDE_PREFIX ${CMAKE_SOURCE_DIR}/core/include)
set(SRC_PREFIX ${CMAKE_SOURCE_DIR}/core/src)

set(INCLUDES_WIDGETS
        ${INCLUDE_PREFIX}
        ${INCLUDE_PREFIX}/cmd
)

set(SRC_COMMAND
#        ${INCLUDE_PREFIX}/cmd/Lexer.h
#        ${SRC_PREFIX}/cmd/Lexer.cpp
)

set(SRC_WIDGETS
        ${SRC_COMMAND}
)