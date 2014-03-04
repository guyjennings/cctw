
# USAGE: Set TCL=/path/to/tcl to override default Tcl installation

TARGET = CctwTcl
TEMPLATE = lib
#CONFIG += uitools debug

include("../cctw.pri")

QT += concurrent core gui network script widgets opengl printsupport svg

target.path=$$prefix/lib
INSTALLS += target

#ldconf.files=qtcl.conf
#ldconf.path=/etc/ls.so.conf.d
#INSTALLS +=ldconf

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

DEFINES += CCTW_VERSION=\"$$VERSION\"
DEFINES += NO_GUI

release {
  QMAKE_CFLAGS_RELEASE += -O2
  QMAKE_CXXFLAGS       += -O2
}

include(../submodules/qceplib/qceplib-base.pri)
include(../submodules/qceplib/qceplib-hdf5.pri)
include(../submodules/qceplib/qceplib-tiff.pri)
include(../submodules/qceplib/qceplib-cbf.pri)
include(../submodules/qceplib/qceplib-mar345.pri)
include(../submodules/qceplib/qceplib-qwt.pri)
#include(../submodules/qceplib/qceplib-nexus.pri)
include(../cctwlib/cctwlib.pri)
#include("../cctwqt/cctwqt.pri")

defined(TCL, var) {
    message("using TCL: $${TCL}")
    INCLUDEPATH += $${TCL}/include
    LIBS += -L$${TCL}/lib -ltcl8.6
} else {
    LIBS += -ltcl
}

HEADERS += \
    cctwtcl_commands.h \
    cctwtcltiff.h

SOURCES += \
    cctwtcl.cpp \
    cctwtcl_commands.cpp \
    cctwtcltiff.cpp

OTHER_FILES += \
    cctw-dummies.h \
    cctw-dummies.cpp \
    cctwtcltest.tcl

PRE_TARGETDEPS += pkgIndex

QMAKE_EXTRA_TARGETS +=pkgIndex

pkgIndex.commands = ( echo "package ifneeded cctw $${VERSION} [list load [file join \\\$$dir libCctwTcl[info sharedlibextension]]]" ) > pkgIndex.tcl
pkgIndex.depends = FORCE

QMAKE_CLEAN += pkgIndex.tcl


