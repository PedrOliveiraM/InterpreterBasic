TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        lexer.cpp \
        main.cpp

DISTFILES += \
    backup \
    basic.txt

HEADERS += \
    lexer.h
