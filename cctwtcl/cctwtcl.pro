TARGET = CctwTcl
TEMPLATE = lib
CONFIG += uitools debug

include("../cctw.pri")

QT += concurrent core gui network script widgets opengl svg

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
include("../cctwqt/qceplib-code/qwt-6.1.pri")
#include("../cctwqt/qceplib-code/qt-nexus.pri")
include("../cctwlib/cctwlib.pri")
include("../cctwqt/cctwqt.pri")

exists($(HOME)/sfw/tcl-8.6.0/include) {
    INCLUDEPATH += $(HOME)/sfw/tcl-8.6.0/include

    LIBS += -L$(HOME)/sfw/tcl-8.6.0/lib -ltcl8.6
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
    cctwtcltest.tcl

