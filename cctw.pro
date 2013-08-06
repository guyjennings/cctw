#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

QT          += core gui network script

TARGET       = cctw
#CONFIG      += console qt
INCLUDEPATH += cctwqt

DEFINES     += USE_QT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

TEMPLATE = app

include(cctwqt/qceplib-code/qceplib.pri)

SOURCES += main.cpp \
    cctwinputdatainterface.cpp \
    cctwoutputdatainterface.cpp \
    cctwswiftinputdata.cpp \
    cctwswiftoutputdata.cpp \
    cctwtransforminterface.cpp \
    cctwtransformer.cpp \
    cctwvector3d.cpp \
    cctwmatrix3x3.cpp \
    cctwcrystalcoordinatetransform.cpp \
    cctwrotationtransform.cpp \
    cctwcompoundtransform.cpp \
    cctwoutputchunkindex.cpp \
    cctwcommandline.cpp \
    cctwchunkeddatainterface.cpp \
    cctwinputchunkindex.cpp \
    cctwchunkindex.cpp \
    cctwqt/cctwqtapplication.cpp \
    cctwqt/cctwqtoutputdata.cpp \
    cctwqt/cctwqtmainwindow.cpp \
    cctwqt/cctwqtinputdata.cpp \
    cctwqt/cctwqtscriptengine.cpp \
    cctwqt/cctwqtsetupinputdialog.cpp \
    cctwqt/cctwqtsetupoutputdialog.cpp \
    cctwqt/cctwqtsetuptransformdialog.cpp \
    cctwqt/cctwqttransformonedialog.cpp \
    cctwqt/cctwqtsetupslicedialog.cpp \
    cctwqt/cctwqtoutputslicedata.cpp \
    cctwqt/cctwqtdataframe.cpp \
    cctwqt/cctwqtdatachunk.cpp \
    cctwqt/cctwqtdataframemanager.cpp \
    cctwqt/cctwqtdataframemanagerthread.cpp \
    cctwqt/cctwqttransformer.cpp \
    cctwqt/cctwqtcrystalcoordinatetransform.cpp \
    cctwqt/cctwqtobject.cpp \
    cctwqt/cctwqtdebug.cpp

HEADERS += \
    cctwinputdatainterface.h \
    cctwoutputdatainterface.h \
    cctwswiftinputdata.h \
    cctwswiftoutputdata.h \
    cctwtransforminterface.h \
    cctwtransformer.h \
    cctwvector3d.h \
    cctwmatrix3x3.h \
    cctwcrystalcoordinatetransform.h \
    cctwrotationtransform.h \
    cctwcompoundtransform.h \
    cctwoutputchunkindex.h \
    cctwcommandline.h \
    cctwchunkeddatainterface.h \
    cctwinputchunkindex.h \
    cctwchunkindex.h \
    cctwqt/cctwqtapplication.h \
    cctwqt/cctwqtoutputdata.h \
    cctwqt/cctwqtmainwindow.h \
    cctwqt/cctwqtinputdata.h \
    cctwqt/cctwqtscriptengine.h \
    cctwqt/cctwqtsetupinputdialog.h \
    cctwqt/cctwqtsetupoutputdialog.h \
    cctwqt/cctwqtsetuptransformdialog.h \
    cctwqt/cctwqttransformonedialog.h \
    cctwqt/cctwqtsetupslicedialog.h \
    cctwqt/cctwqtoutputslicedata.h \
    cctwqt/cctwqtdataframe.h \
    cctwqt/cctwqtdatachunk.h \
    cctwqt/cctwqtdataframemanager.h \
    cctwqt/cctwqtdataframemanagerthread.h \
    cctwqt/cctwqttransformer.h \
    cctwqt/cctwqtcrystalcoordinatetransform.h \
    cctwqt/cctwqtobject.h \
    cctwqt/cctwqtdebug.h

FORMS += \
    cctwqt/cctwqtmainwindow.ui \
    cctwqt/cctwqtsetupinputdialog.ui \
    cctwqt/cctwqtsetupoutputdialog.ui \
    cctwqt/cctwqtsetuptransformdialog.ui \
    cctwqt/cctwqttransformonedialog.ui \
    cctwqt/cctwqtsetupslicedialog.ui

OTHER_FILES += Doxyfile

QMAKE_EXTRA_TARGETS += dox

dox.commands = "("
dox.commands += cat $${PWD}/Doxyfile ;
dox.commands += echo "PROJECT_NUMBER=$${VERSION}" ;
dox.commands += echo "INPUT=\"$${PWD}\"" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/cctwqt/" ;
dox.commands += echo "ALIASES=\"projectnumber=$${VERSION}\""
dox.commands += ")" > Doxyfile.out ;
dox.commands += doxygen < Doxyfile.out -
dox.depends = FORCE $${PWD}/Doxyfile
