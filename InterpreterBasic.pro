TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        lexer.cpp \
        main.cpp \
        parser.cpp \
        semantic.cpp

DISTFILES += \
    basic.txt \
    semanticCPP

HEADERS += \
    lexer.h \
    parser.h \
    semantic.h
