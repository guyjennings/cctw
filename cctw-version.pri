CCTW_VERSION = 0.2.3

VERSION = $${CCTW_VERSION}

exists(cctw-config-local.pri) {
    message(reading local config)
    include(cctw-config-local.pri)
} else {
    message(reading default config)
    include(cctw-config-default.pri)
}

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

#unix:!macx {
#  LIBS += -ltcmalloc
#}

INCLUDE_IMPORT_COMMANDS {
    DEFINES += WANT_IMPORT_COMMANDS
}

INCLUDE_ANALYSIS_COMMANDS {
    DEFINES += WANT_ANALYSIS_COMMANDS
}
