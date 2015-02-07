
INCLUDEPATH += $${PWD}

HEADERS += \
    $${PWD}/cctwtcl_commands.h \
    $${PWD}/cctwtcltiff.h

SOURCES += \
    $${PWD}/cctwtcl.cpp \
    $${PWD}/cctwtcl_commands.cpp \
    $${PWD}/cctwtcltiff.cpp

OTHER_FILES += \
    $${PWD}/cctw-dummies.h \
    $${PWD}/cctw-dummies.cpp \
    $${PWD}/cctwtcltest.tcl
