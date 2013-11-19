INCLUDEPATH += $${PWD}

SOURCES += \
    $${PWD}/cctwqtapplication.cpp \
    $${PWD}/cctwqtoutputdata.cpp \
    $${PWD}/cctwqtmainwindow.cpp \
    $${PWD}/cctwqtinputdata.cpp \
    $${PWD}/cctwqtsetupinputdialog.cpp \
    $${PWD}/cctwqtsetupoutputdialog.cpp \
    $${PWD}/cctwqtsetuptransformdialog.cpp \
    $${PWD}/cctwqttransformonedialog.cpp \
    $${PWD}/cctwqtsetupslicedialog.cpp \
    $${PWD}/cctwqtoutputslicedata.cpp \
    $${PWD}/cctwqtdataframe.cpp \
    $${PWD}/cctwqtdatachunk.cpp \
    $${PWD}/cctwqttransformer.cpp \
    $${PWD}/cctwqtcrystalcoordinatetransform.cpp \
    $${PWD}/cctwqtchunkeddata.cpp \
    $${PWD}/cctwqtcrystalcoordinateparameters.cpp \
    $${PWD}/cctwqtpeingresscommand.cpp

HEADERS += \
    $${PWD}/cctwqtapplication.h \
    $${PWD}/cctwqtoutputdata.h \
    $${PWD}/cctwqtmainwindow.h \
    $${PWD}/cctwqtinputdata.h \
    $${PWD}/cctwqtsetupinputdialog.h \
    $${PWD}/cctwqtsetupoutputdialog.h \
    $${PWD}/cctwqtsetuptransformdialog.h \
    $${PWD}/cctwqttransformonedialog.h \
    $${PWD}/cctwqtsetupslicedialog.h \
    $${PWD}/cctwqtoutputslicedata.h \
    $${PWD}/cctwqtdataframe.h \
    $${PWD}/cctwqtdatachunk.h \
    $${PWD}/cctwqttransformer.h \
    $${PWD}/cctwqtcrystalcoordinatetransform.h \
    $${PWD}/cctwqtchunkeddata.h \
    $${PWD}/cctwqtcrystalcoordinateparameters.h \
    $${PWD}/cctwqtpeingresscommand.h

FORMS += \
    $${PWD}/cctwqtmainwindow.ui \
    $${PWD}/cctwqtsetupinputdialog.ui \
    $${PWD}/cctwqtsetupoutputdialog.ui \
    $${PWD}/cctwqtsetuptransformdialog.ui \
    $${PWD}/cctwqttransformonedialog.ui \
    $${PWD}/cctwqtsetupslicedialog.ui

message(INCLUDEPATH = $${INCLUDEPATH})
