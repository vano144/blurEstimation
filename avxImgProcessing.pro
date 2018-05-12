QT += gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_FLAGS_RELEASE += -O3 -mavx

QMAKE_CXXFLAGS_RELEASE += -O3 -mavx

QMAKE_FLAGS_DEBUG += -O3 -mavx

QMAKE_CXXFLAGS_DEBUG += -O3 -mavx

QMAKE_FLAGS_PROFILE += -O3 -mavx

QMAKE_CXXFLAGS_PROFILE += -O3 -mavx

#QMAKE_CXXFLAGS += -O3 -mavx
#QMAKE_FLAGS += -O3 -mavx

SOURCES += \
        main.cpp \
    imagereader.cpp \
    imgapi.cpp \
    avximgapi.cpp \
    avximgver2.cpp \
    imagereaderver2.cpp

HEADERS += \
    imagereader.h \
    config.h \
    imgapi.h \
    avximgapi.h \
    avximgver2.h \
    imagereaderver2.h
