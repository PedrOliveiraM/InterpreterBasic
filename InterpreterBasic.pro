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
    ImprimirNumeros.txt \
    Media.txt \
    NumMaior.txt \
    OperacaoArit.txt

HEADERS += \
    lexer.h \
    parser.h \
    semantic.h
