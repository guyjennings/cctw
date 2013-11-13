INCLUDEPATH += $${PWD}

SOURCES += \
    $${PWD}/cctwqtapplication.cpp \
    $${PWD}/cctwqtoutputdata.cpp \
    $${PWD}/cctwqtmainwindow.cpp \
    $${PWD}/cctwqtinputdata.cpp \
    $${PWD}/cctwqtscriptengine.cpp \
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
    $${PWD}/cctwqtobject.cpp \
    $${PWD}/cctwqtdebug.cpp \
    $${PWD}/cctwqtchunkeddata.cpp \
    $${PWD}/cctwtransformtest.cpp \
    $${PWD}/cctwqtdoublevector3dproperty.cpp \
    $${PWD}/cctwqtdoublematrix3x3property.cpp \
    $${PWD}/cctwqtunitcellproperty.cpp \
    $${PWD}/cctwqtcrystalcoordinateparameters.cpp \
    $${PWD}/cctwqtintvector3dproperty.cpp \
    $${PWD}/cctwqtinputdataframemanager.cpp \
    $${PWD}/cctwqtoutputdataframemanager.cpp \
    $${PWD}/cctwqtdataframemanager.cpp \
    $${PWD}/cctwqtpeingresscommand.cpp  \
    $${PWD}/cctwqtlinearfitter.cpp \
    $${PWD}/cctwqtthread.cpp

HEADERS += \
    $${PWD}/cctwqtapplication.h \
    $${PWD}/cctwqtoutputdata.h \
    $${PWD}/cctwqtmainwindow.h \
    $${PWD}/cctwqtinputdata.h \
    $${PWD}/cctwqtscriptengine.h \
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
    $${PWD}/cctwqtobject.h \
    $${PWD}/cctwqtdebug.h \
    $${PWD}/cctwqtchunkeddata.h \
    $${PWD}/cctwtransformtest.h \
    $${PWD}/cctwqtdoublevector3dproperty.h \
    $${PWD}/cctwqtdoublematrix3x3property.h \
    $${PWD}/cctwqtunitcellproperty.h \
    $${PWD}/cctwqtcrystalcoordinateparameters.h \
    $${PWD}/cctwqtintvector3dproperty.h \
    $${PWD}/cctwqtinputdataframemanager.h \
    $${PWD}/cctwqtoutputdataframemanager.h \
    $${PWD}/cctwqtdataframemanager.h \
    $${PWD}/cctwqtpeingresscommand.h \
    $${PWD}/cctwqtlinearfitter.h \
    $${PWD}/cctwqtthread.h

FORMS += \
    $${PWD}/cctwqtmainwindow.ui \
    $${PWD}/cctwqtsetupinputdialog.ui \
    $${PWD}/cctwqtsetupoutputdialog.ui \
    $${PWD}/cctwqtsetuptransformdialog.ui \
    $${PWD}/cctwqttransformonedialog.ui \
    $${PWD}/cctwqtsetupslicedialog.ui

message(INCLUDEPATH = $${INCLUDEPATH})
