#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

TEMPLATE = app

include(../cctw.pri)

QT          += core gui network script

TARGET       = cctwqt
DESTDIR      = ../

INCLUDEPATH += . ../cctwlib/

DEFINES     += USE_QT
DEFINES     += CCTW_VERSION=\"$$VERSION\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

include(qceplib-code/qceplib.pri)

include(../cctwlib/cctwlib.pri)

SOURCES += main.cpp \
    cctwqtapplication.cpp \
    cctwqtoutputdata.cpp \
    cctwqtmainwindow.cpp \
    cctwqtinputdata.cpp \
    cctwqtscriptengine.cpp \
    cctwqtsetupinputdialog.cpp \
    cctwqtsetupoutputdialog.cpp \
    cctwqtsetuptransformdialog.cpp \
    cctwqttransformonedialog.cpp \
    cctwqtsetupslicedialog.cpp \
    cctwqtoutputslicedata.cpp \
    cctwqtdataframe.cpp \
    cctwqtdatachunk.cpp \
    cctwqttransformer.cpp \
    cctwqtcrystalcoordinatetransform.cpp \
    cctwqtobject.cpp \
    cctwqtdebug.cpp \
    cctwqtchunkeddata.cpp \
    cctwtransformtest.cpp \
    cctwqtdoublevector3dproperty.cpp \
    cctwqtdoublematrix3x3property.cpp \
    cctwqtunitcellproperty.cpp \
    cctwqtcrystalcoordinateparameters.cpp \
    cctwqtintvector3dproperty.cpp \
    cctwqtinputdataframemanager.cpp \
    cctwqtoutputdataframemanager.cpp \
    cctwqtdataframemanager.cpp \
    cctwqtthread.cpp

HEADERS += \
    cctwqtapplication.h \
    cctwqtoutputdata.h \
    cctwqtmainwindow.h \
    cctwqtinputdata.h \
    cctwqtscriptengine.h \
    cctwqtsetupinputdialog.h \
    cctwqtsetupoutputdialog.h \
    cctwqtsetuptransformdialog.h \
    cctwqttransformonedialog.h \
    cctwqtsetupslicedialog.h \
    cctwqtoutputslicedata.h \
    cctwqtdataframe.h \
    cctwqtdatachunk.h \
    cctwqttransformer.h \
    cctwqtcrystalcoordinatetransform.h \
    cctwqtobject.h \
    cctwqtdebug.h \
    cctwqtchunkeddata.h \
    cctwtransformtest.h \
    cctwqtdoublevector3dproperty.h \
    cctwqtdoublematrix3x3property.h \
    cctwqtunitcellproperty.h \
    cctwqtcrystalcoordinateparameters.h \
    cctwqtintvector3dproperty.h \
    cctwqtinputdataframemanager.h \
    cctwqtoutputdataframemanager.h \
    cctwqtdataframemanager.h \
    cctwqtthread.h

FORMS += \
    cctwqtmainwindow.ui \
    cctwqtsetupinputdialog.ui \
    cctwqtsetupoutputdialog.ui \
    cctwqtsetuptransformdialog.ui \
    cctwqttransformonedialog.ui \
    cctwqtsetupslicedialog.ui

