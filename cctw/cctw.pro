TEMPLATE = app

include(../cctw-version.pri)

QT += script

TARGET = cctw

DEFINES     += NO_GUI

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += concurrent
}

macx {
  CONFIG -= app_bundle
}

include(../submodules/qceplib/qceplib-qwt.pri)
include(../submodules/qceplib/qceplib-base.pri)
include(../submodules/qceplib/qceplib-szip.pri)
include(../submodules/qceplib/qceplib-zlib.pri)
include(../submodules/qceplib/qceplib-hdf5.pri)

INCLUDE_IMPORT_COMMANDS {
include(../submodules/qceplib/qceplib-tiff.pri)
include(../submodules/qceplib/qceplib-cbf.pri)
include(../submodules/qceplib/qceplib-mar345.pri)
}

#include(../submodules/qceplib/qceplib-nexus.pri)
include(../cctwlib/cctwlib.pri)

include(cctw.pri)

SUBDIRS += \
    ../cctwtest/cctwtest.pro
