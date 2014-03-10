TEMPLATE = lib

include(../cctw-version.pri)

QT += core script

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += concurrent
}

DEFINES += CCTW_VERSION=\"$$VERSION\"
DEFINES += NO_GUI

include(../submodules/qceplib/qceplib-base.pri)
include(../submodules/qceplib/qceplib-hdf5.pri)
include(../submodules/qceplib/qceplib-tiff.pri)
include(../submodules/qceplib/qceplib-cbf.pri)
include(../submodules/qceplib/qceplib-mar345.pri)
include(../submodules/qceplib/qceplib-qwt.pri)
#include(../submodules/qceplib/qceplib-nexus.pri)

include(cctwlib.pri)
