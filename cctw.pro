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
    cctwqt/cctwtqinputdata.cpp \
    cctwqt/cctwqtoutputdata.cpp \
    cctwqt/cctwqtmainwindow.cpp

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
    cctwqt/cctwtqinputdata.h \
    cctwqt/cctwqtoutputdata.h \
    cctwqt/cctwqtmainwindow.h

FORMS += \
    cctwqt/cctwqtmainwindow.ui
