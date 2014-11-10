#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}


include(cctw-version.pri)

TEMPLATE = subdirs
BUILD_CCTWQT:  SUBDIRS  = cctwqt

unix {
BUILD_CCTWLIB: SUBDIRS += cctwlib
BUILD_CCTWTCL: SUBDIRS += cctwtcl
BUILD_CCTWCLI: SUBDIRS += cctwcli
}

#unix:!macx {
#BUILD_CCTWMPI: SUBDIRS += cctwmpi
#}

OTHER_FILES += Doxyfile \
    cctw.dox \
    doc/Doxyfile \
    doc/findphi.nb \
    doc/pznpt4_march2007.txt \
    doc/transformtest.f \
    doc/transformtools.f \
    doc/standards.doc \
    doc/pznpt4_march2007.out.txt \
    tests/testTransformScaling.js

OTHER_FILES += \
    ../specmacros/dummyscan.mac \
    ../specmacros/osborn.fits \
    ../specmacros/spotIntegrator.js \
    ../specmacros/ff1scan.dat \
    ../specmacros/fb1scan.dat \
    ../specmacros/ff2scan.dat \
    ../specmacros/fb2scan.dat \
    ../specmacros/sbscan.dat \
    ../specmacros/sfscan.dat \
    ../specmacros/ubfscan.dat \
    ../specmacros/ubbscan.dat \
    ../specmacros/aggregatedMask.js

win32 { # Copy QT Libraries into app directory
#  PRE_TARGETDEPS += app
#  QMAKE_EXTRA_TARGETS += app
  LIBDIR = $$[QT_INSTALL_BINS]
  LIBDIR_WIN = $${replace(LIBDIR, /, \\)}

  QMAKE_EXTRA_TARGETS += qtlibs

  isEqual(QT_MAJOR_VERSION, 5) {
    CONFIG(debug, debug|release) {
      libs =  Qt5Cored \
              Qt5Networkd \
              Qt5Guid \
              Qt5Scriptd \
              Qt5Widgetsd \
              Qt5Svgd \
              Qt5OpenGLd \
              Qt5PrintSupportd \
              icudt51 \
              icuin51 \
              icuuc51 \
              libEGLd \
              libGLESv2d
      platform = qwindowsd
    } else {
      libs =  Qt5Core \
              Qt5Network \
              Qt5Gui \
              Qt5Script \
              Qt5Widgets \
              Qt5Svg \
              Qt5OpenGL \
              Qt5PrintSupport \
              icudt51 \
              icuin51 \
              icuuc51 \
              libEGL \
              libGLESv2
      platform = qwindows
    }

    QMAKE_EXTRA_TARGETS += qtplatformdir
    qtplatformdir.target = bin/platforms
    qtplatformdir.commands = if not exist ..\\platforms $(MKDIR) ..\\platforms

    QMAKE_EXTRA_TARGETS += qtplatform
    qtplatform.target   = bin/platforms/$${platform}.dll
    qtplatform.depends  = qtplatformdir $${LIBDIR_WIN}/../plugins/platforms/$${platform}.dll
    qtplatform.commands +=
    qtplatform.commands += $(COPY_FILE) $${LIBDIR_WIN}\\..\\plugins\\platforms\\$${platform}.dll bin\\platforms\\$${platform}.dll

    qtlibs.depends += qtplatform

    for(lib, libs) {
      !build_pass:message(Target $${lib})
      QMAKE_EXTRA_TARGETS += $${lib}
      $${lib}.target      = bin/$${lib}.dll
      $${lib}.depends    += $${LIBDIR}/$${lib}.dll
      $${lib}.commands   += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll bin\\$${lib}.dll &

      qtlibs.depends     += $${lib}
    }

    QMAKE_CLEAN += bin/platforms/*
    QMAKE_CLEAN += bin/platforms
  }

  isEqual(QT_MAJOR_VERSION, 4) {
    CONFIG(debug, debug|release) {
      libs = QtCored4 QtNetworkd4 QtGuid4 QtScriptd4 QtOpenGLd4 QtSvgd4
    } else {
      libs = QtCore4 QtNetwork4 QtGui4 QtScript4 QtOpenGL4 QtSvg4
    }

    for(lib, libs) {
      !build_pass:message(Target $${lib})

      qtlibs.depends     += $${LIBDIR}/$${lib}.dll
      qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll bin\\$${lib}.dll &
    }
  }

  QMAKE_CLEAN += ../*.dll
  QMAKE_CLEAN += ../plugins/*
  QMAKE_CLEAN += ../plugins

  PRE_TARGETDEPS  += qtlibs
}

TARGET.depends += qtlibs

QMAKE_EXTRA_TARGETS += dox

dox.commands =  ( cat $${PWD}/Doxyfile ; \
                  echo "PROJECT_NAME=\"$${TARGET}\"" ; \
                  echo "PROJECT_NUMBER=$${VERSION}" ; \
                  echo "INPUT+=\"$${PWD}\"/cctw.dox" ; \
                  echo "INPUT+=\"$${PWD}\"/cctwlib/" ; \
                  echo "INPUT+=\"$${PWD}\"/cctwcli/" ; \
                  echo "INPUT+=\"$${PWD}\"/cctwqt/" ; \
                  echo "INPUT+=\"$${PWD}\"/cctwtcl/" ; \
                  echo "ALIASES=\"projectnumber=$${VERSION}\"" \
                ) > Doxyfile.out ;
dox.commands += doxygen < Doxyfile.out -

dox.depends = FORCE $${PWD}/Doxyfile

QMAKE_EXTRA_TARGETS += upload-dox

upload-dox.commands = rsync -e ssh -av --del dox/html/ guyjennings,$${TARGET}@web.sourceforge.net:htdocs/

QMAKE_EXTRA_TARGETS += tarball

tarball.depends = FORCE $${TARGET}.spec

tarball.commands += \
      rm -rf $${TARGET}-$${VERSION} ; \
      $(MKDIR) $${TARGET}-$${VERSION} && \

tarball.commands += \
      $(COPY_FILE) -R $${PWD}/* $${TARGET}-$${VERSION}/ && \
      $(COPY_FILE) $${TARGET}.spec -t $${TARGET}-$${VERSION} &&

tarball.commands += \
  tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}

QMAKE_EXTRA_TARGETS += specfile

specfile.target = $${TARGET}.spec
specfile.depends = $${PWD}/$${TARGET}.spec.in $${PWD}/$${TARGET}-version.pri
specfile.commands = perl -p -e '\'s/Version:.*/Version: $${VERSION}/\'' $${PWD}/$${TARGET}.spec.in > $${TARGET}.spec
