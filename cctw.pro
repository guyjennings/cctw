#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T11:18:12
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

include(cctw.pri)

TEMPLATE = subdirs
SUBDIRS  = cctwqt

unix {
SUBDIRS += cctwtcl
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

QMAKE_EXTRA_TARGETS += dox

dox.commands =  ( cat $${PWD}/Doxyfile ; \
                  echo "PROJECT_NAME=\"$${TARGET}\"" ; \
                  echo "PROJECT_NUMBER=$${VERSION}" ; \
                  echo "INPUT=\"$${PWD}\"/cctwlib/" ; \
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
