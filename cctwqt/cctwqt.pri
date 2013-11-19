INCLUDEPATH += $${PWD}

SOURCES += \
    $${PWD}/cctwqtapplication.cpp \
    $${PWD}/cctwqtmainwindow.cpp \
    $${PWD}/cctwqtsetupinputdialog.cpp \
    $${PWD}/cctwqtsetupoutputdialog.cpp \
    $${PWD}/cctwqtsetuptransformdialog.cpp \
    $${PWD}/cctwqttransformonedialog.cpp \
    $${PWD}/cctwqtsetupslicedialog.cpp \
    $${PWD}/cctwqtdatachunk.cpp \
    $${PWD}/cctwqttransformer.cpp \
    $${PWD}/cctwqtcrystalcoordinatetransform.cpp \
    $${PWD}/cctwqtpeingresscommand.cpp

HEADERS += \
    $${PWD}/cctwqtapplication.h \
    $${PWD}/cctwqtmainwindow.h \
    $${PWD}/cctwqtsetupinputdialog.h \
    $${PWD}/cctwqtsetupoutputdialog.h \
    $${PWD}/cctwqtsetuptransformdialog.h \
    $${PWD}/cctwqttransformonedialog.h \
    $${PWD}/cctwqtsetupslicedialog.h \
    $${PWD}/cctwqtdatachunk.h \
    $${PWD}/cctwqttransformer.h \
    $${PWD}/cctwqtcrystalcoordinatetransform.h \
    $${PWD}/cctwqtpeingresscommand.h

FORMS += \
    $${PWD}/cctwqtmainwindow.ui \
    $${PWD}/cctwqtsetupinputdialog.ui \
    $${PWD}/cctwqtsetupoutputdialog.ui \
    $${PWD}/cctwqtsetuptransformdialog.ui \
    $${PWD}/cctwqttransformonedialog.ui \
    $${PWD}/cctwqtsetupslicedialog.ui

message(INCLUDEPATH = $${INCLUDEPATH})
