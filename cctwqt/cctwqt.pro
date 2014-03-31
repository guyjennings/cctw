#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

TEMPLATE = app

include(../cctw-version.pri)

QT          += core gui network script opengl svg

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += widgets concurrent printsupport
}

TARGET       = cctwqt

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
