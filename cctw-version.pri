CCTW_VERSION = 0.1.1

VERSION = $${CCTW_VERSION}

DESTDIR      = ../bin/

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

DEFINES += CCTW_VERSION=\"$$VERSION\"

DEFINES += CCTW_WANT_MPI

win32 {
  DEFINES += NOMINMAX
}

*g++* {
    QMAKE_CXXFLAGS += -g
    QMAKE_CFLAGS += -g
    QMAKE_LFLAGS += -g
}

unix:!macx {
  LIBS += -ltcmalloc
}
