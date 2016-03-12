#--------------------------------------------------------
#
# Pro file for Android and Windows builds with Qt Creator
#
#--------------------------------------------------------

QT += core

TEMPLATE = lib
TARGET = libhackrf

CONFIG(MINGW32):LIBHACKRFSRC = "D:\softs\hackrf\host\libhackrf"
INCLUDEPATH += $$LIBHACKRFSRC/src

CONFIG(MINGW32):INCLUDEPATH += "D:\libusb-1.0.19\include\libusb-1.0"

SOURCES = $$LIBHACKRFSRC/src/hackrf.c

HEADERS = $$LIBHACKRFSRC/src/hackrf.h

CONFIG(MINGW32):LIBS += -LD:\libusb-1.0.19\MinGW32\dll -llibusb-1.0

CONFIG(ANDROID):CONFIG += mobility
CONFIG(ANDROID):MOBILITY =
