#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

TEMPLATE = app

include(../cctw.pri)

QT          += concurrent core gui network script

TARGET       = cctwqt

INCLUDEPATH += . ../cctwlib/

DEFINES     += USE_QT
DEFINES     += CCTW_VERSION=\"$$VERSION\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

include(qceplib-code/qceplib.pri)
include(qceplib-code/qt-hdf5.pri)
#include(qceplib-code/qt-nexus.pri)
include(../cctwlib/cctwlib.pri)
include(cctwqt.pri)

SOURCES += main.cpp

OTHER_FILES += \
    cctwqt.pri

