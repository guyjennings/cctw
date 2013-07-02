#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cctw
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    cctwinputdatainterface.cpp \
    cctwoutputdatainterface.cpp \
    cctwswiftinputdata.cpp \
    cctwswiftoutputdata.cpp \
    cctwtransforminterface.cpp \
    cctwtransformer.cpp \
    cctwvector3d.cpp \
    cctwmatrix3x3.cpp

HEADERS += \
    cctwinputdatainterface.h \
    cctwoutputdatainterface.h \
    cctwswiftinputdata.h \
    cctwswiftoutputdata.h \
    cctwtransforminterface.h \
    cctwtransformer.h \
    cctwvector3d.h \
    cctwmatrix3x3.h
