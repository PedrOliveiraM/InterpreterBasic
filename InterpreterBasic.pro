TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        lexer.cpp \
        main.cpp \
        parser.cpp

DISTFILES += \
    backup \
    basic.txt

HEADERS += \
    lexer.h \
    parser.h
