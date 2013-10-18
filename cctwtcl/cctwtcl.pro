TARGET = CctwTcl
TEMPLATE = lib
CONFIG += uitools debug

include("../cctw.pri")

QT += core gui network script

target.path=$$prefix/lib
INSTALLS += target

#ldconf.files=qtcl.conf
#ldconf.path=/etc/ls.so.conf.d
#INSTALLS +=ldconf

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

DEFINES += USE_QT
DEFINES += CCTW_VERSION=\"$$VERSION\"

include("../cctwqt/qceplib-code/qceplib.pri")
include("../cctwqt/qceplib-code/qt-hdf5.pri")
#include("../cctwqt/qceplib-code/qt-nexus.pri")
include("../cctwlib/cctwlib.pri")
include("../cctwqt/cctwqt.pri")

HEADERS +=

SOURCES +=

