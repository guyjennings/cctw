TEMPLATE = app

include(../cctw_version.pri)

QT += core script

TARGET = cctwcli

DESTDIR = ../bin/

DEFINES     += CCTW_VERSION=\"$$VERSION\"
DEFINES     += NO_GUI

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += concurrent
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

SOURCES += cctwcli.cpp
