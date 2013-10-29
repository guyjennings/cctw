TARGET = CctwTcl
TEMPLATE = lib
CONFIG += uitools debug

include("../cctw.pri")

QT += concurrent core gui network script widgets

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
DEFINES += NO_GUI

release {
  QMAKE_CFLAGS_RELEASE += -O2
  QMAKE_CXXFLAGS       += -O2
}

include("../cctwqt/qceplib-code/qceplib.pri")
include("../cctwqt/qceplib-code/qt-hdf5.pri")
#include("../cctwqt/qceplib-code/qt-nexus.pri")
include("../cctwlib/cctwlib.pri")
include("../cctwqt/cctwqt.pri")

LIBS += -ltcl

HEADERS += \
    cctwtcl_commands.h

SOURCES += \
    cctwtcl.cpp \
    cctwtcl_commands.cpp

OTHER_FILES += \
    cctwtcltest.tcl

