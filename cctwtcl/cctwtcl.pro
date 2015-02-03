
# USAGE: Set TCL=/path/to/tcl to override default Tcl installation

TARGET = CctwTcl
TEMPLATE = lib
#CONFIG += uitools debug

include(../cctw-version.pri)

QT += core script

greaterThan(QT_MAJOR_VERSION, 4) {
 QT += concurrent
}

#target.path=$$prefix/lib
#INSTALLS += target

#ldconf.files=qtcl.conf
#ldconf.path=/etc/ls.so.conf.d
#INSTALLS +=ldconf

DEFINES += NO_GUI

release {
  QMAKE_CFLAGS_RELEASE += -O2
  QMAKE_CXXFLAGS       += -O2
}

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
include(cctwtcl.pri)

defined(TCL, var) {
    message("using TCL: $${TCL}")
    INCLUDEPATH += $${TCL}/include
    LIBS += -L$${TCL}/lib -ltcl8.6
} else {
    # This is the include path for APT package tcl-dev
    INCLUDEPATH += /usr/include/tcl8.6
    LIBS += -ltcl
}

pkgIndex.commands = ( echo "package ifneeded cctw $${VERSION} [list load [file join \\\$$dir libCctwTcl[info sharedlibextension]]]" ) > $${DESTDIR}/pkgIndex.tcl
pkgIndex.depends  = $${PWD}/../cctw-version.pri

PRE_TARGETDEPS += pkgIndex

QMAKE_EXTRA_TARGETS += pkgIndex

QMAKE_CLEAN += $${DESTDIR}/pkgIndex.tcl


