TEMPLATE = app

include(../cctw-version.pri)

QT += core script

TARGET = cctwmpi

DEFINES     += NO_GUI

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += concurrent
}

QMAKE_CC  = mpicc -DCCTW_WANT_MPI
QMAKE_CXX = mpicxx -DCCTW_WANT_MPI
QMAKE_LINK = mpicxx -DCCTW_WANT_MPI

include(../submodules/qceplib/qceplib-base.pri)
include(../submodules/qceplib/qceplib-szip.pri)
include(../submodules/qceplib/qceplib-zlib.pri)
include(../submodules/qceplib/qceplib-hdf5.pri)
include(../submodules/qceplib/qceplib-tiff.pri)
include(../submodules/qceplib/qceplib-cbf.pri)
include(../submodules/qceplib/qceplib-mar345.pri)
#include(../submodules/qceplib/qceplib-qwt.pri)
#include(../submodules/qceplib/qceplib-nexus.pri)
include(../cctwlib/cctwlib.pri)

include(cctwmpi.pri)
