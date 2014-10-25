TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/midi/midi.c \
    src/main.c

HEADERS += \
    src/midi/midi.h

OTHER_FILES += \
    src/Makefile
