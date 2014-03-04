#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

TEMPLATE = app

include(../cctw-version.pri)

QT          += core gui network script opengl svg

TARGET       = cctwqt

DESTDIR      = ../bin/

DEFINES     += CCTW_VERSION=\"$$VERSION\"

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += widgets concurrent printsupport
}

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

include(../submodules/qceplib/qceplib-base.pri)
include(../submodules/qceplib/qceplib-hdf5.pri)
include(../submodules/qceplib/qceplib-tiff.pri)
include(../submodules/qceplib/qceplib-cbf.pri)
include(../submodules/qceplib/qceplib-mar345.pri)
include(../submodules/qceplib/qceplib-qwt.pri)
#include(../submodules/qceplib/qceplib-nexus.pri)
include(../cctwlib/cctwlib.pri)
include(cctwqt.pri)

SOURCES += main.cpp

OTHER_FILES += \
    cctwqt.pri


