#ifndef CCTWIMPORTDATA_H
#define CCTWIMPORTDATA_H

#include "cctwobject.h"

class CctwApplication;

class CctwImportData : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwImportData(CctwApplication *application,
                          QObject *parent = 0);

signals:

public slots:

private:
  CctwApplication *m_Application;

private:
  Q_PROPERTY(int dataFormat READ get_DataFormat WRITE set_DataFormat)
  QCEP_INTEGER_PROPERTY(DataFormat)

  Q_PROPERTY(QString darkImagePath READ get_DarkImagePath WRITE set_DarkImagePath)
  QCEP_STRING_PROPERTY(DarkImagePath)

  Q_PROPERTY(QStringList imagePaths READ get_ImagePaths WRITE set_ImagePaths)
  QCEP_STRING_LIST_PROPERTY(ImagePaths)

  Q_PROPERTY(QString imageDirectory READ get_ImageDirectory WRITE set_ImageDirectory)
  QCEP_STRING_PROPERTY(ImageDirectory)

  Q_PROPERTY(QString imagePattern READ get_ImagePattern WRITE set_ImagePattern)
  QCEP_STRING_PROPERTY(ImagePattern)
};

#endif // CCTWIMPORTDATA_H
