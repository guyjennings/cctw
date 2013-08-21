#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

VERSION      = 0.1.0
QT          += core gui network script

TARGET       = cctw
#CONFIG      += console qt
INCLUDEPATH += cctwqt

DEFINES     += USE_QT
DEFINES     += CCTW_VERSION=\"$$VERSION\"

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
    cctwqt/cctwqtdebug.cpp \
    cctwqt/cctwqtchunkeddata.cpp

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
    cctwqt/cctwqtdebug.h \
    cctwqt/cctwqtchunkeddata.h

FORMS += \
    cctwqt/cctwqtmainwindow.ui \
    cctwqt/cctwqtsetupinputdialog.ui \
    cctwqt/cctwqtsetupoutputdialog.ui \
    cctwqt/cctwqtsetuptransformdialog.ui \
    cctwqt/cctwqttransformonedialog.ui \
    cctwqt/cctwqtsetupslicedialog.ui

OTHER_FILES += Doxyfile \
    cctw.dox

QMAKE_EXTRA_TARGETS += dox

dox.commands =  ( cat $${PWD}/Doxyfile ; \
                  echo "PROJECT_NAME=\"$${TARGET}\"" ; \
                  echo "PROJECT_NUMBER=$${VERSION}" ; \
                  echo "INPUT=\"$${PWD}\"" ; \
                  echo "INPUT+=\"$${PWD}\"/cctwqt/" ; \
                  echo "ALIASES=\"projectnumber=$${VERSION}\"" \
                ) > Doxyfile.out ;
dox.commands += doxygen < Doxyfile.out -

dox.depends = FORCE $${PWD}/Doxyfile

QMAKE_EXTRA_TARGETS += upload-dox

upload-dox.commands = rsync -e ssh -av --del dox/html/ guyjennings,$${TARGET}@web.sourceforge.net:htdocs/

QMAKE_EXTRA_TARGETS += tarball

tarball.depends = FORCE

tarball.commands += \
      rm -rf $${TARGET}-$${VERSION} ; \
      $(MKDIR) $${TARGET}-$${VERSION} && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/macx && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/win32 && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiff-3.8.2 && \
      $(MKDIR) $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiff-3.8.2/libtiff && \
      $(MKDIR) $${TARGET}-$${VERSION}/maint && \
      $(MKDIR) $${TARGET}-$${VERSION}/swift && \
      $(MKDIR) $${TARGET}-$${VERSION}/swift/tests && \


tarball.commands += \
      $(COPY_FILE) $${PWD}/{Doxyfile,*.{cpp,h,pro,in,txt,sh,ods,ac}} -t $${TARGET}-$${VERSION}/ &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/* \
                      -t $${TARGET}-$${VERSION}/cctwqt &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/tiffconfig/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/tiffconfig/macx/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/macx &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/tiffconfig/win32/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/win32 &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/tiffconfig/tiff-3.8.2/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/tiff-3.8.2 &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/cctwqt/qceplib-code/tiffconfig/tiff-3.8.2/libtiff/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/cctwqt/qceplib-code/tiffconfig/tiff-3.8.2/libtiff &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/maint/* \
                      -t $${TARGET}-$${VERSION}/maint/ &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/swift/* \
                      -t $${TARGET}-$${VERSION}/swift/ &&

tarball.commands += \
      $(COPY_FILE)  $${PWD}/swift/tests/* \
                      -t $${TARGET}-$${VERSION}/swift/tests/ &&

tarball.commands += \
  tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}
