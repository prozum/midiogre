TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/midi.c \
    src/main.c

HEADERS += \
    src/midi.h

OTHER_FILES += \
    src/Makefile
