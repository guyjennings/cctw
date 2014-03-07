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
SUBDIRS  = cctwlib cctwqt

unix {
SUBDIRS += cctwtcl cctwcli
}

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

tarball.depends = FORCE

tarball.commands += \
      rm -rf $${TARGET}-$${VERSION} ; \
      $(MKDIR) $${TARGET}-$${VERSION} && \

tarball.commands += \
      $(COPY_FILE) -R $${PWD}/* $${TARGET}-$${VERSION}/ &&

tarball.commands += \
  tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}
