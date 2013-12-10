#ifndef CCTWIMPORTDATA_H
#define CCTWIMPORTDATA_H

#include "cctwobject.h"
#include "qcepimagedata.h"
#include <QSemaphore>

class CctwApplication;

class CctwImportData : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwImportData(CctwApplication *application,
                          QObject *parent = 0);

signals:

public slots:
  void importData();
  void clearInputFiles();
  void changeDirectory(QString path);
  void appendInputFile(QString path);
  void appendMatchingFiles(QString pattern);

private slots:
  void importDataFrame(int num, QString path);

private:
  bool createOutputFile();
  void closeOutputFile();
  void writeOutputFrame(int num, QcepImageData<double> *img);

private:
  CctwApplication *m_Application;
  QMutex           m_OutputMutex;
  QSemaphore       m_BacklogSemaphore;
  QSemaphore       m_CompletionSemaphore;

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

  Q_PROPERTY(QString outputPath READ get_OutputPath WRITE set_OutputPath)
  QCEP_STRING_PROPERTY(OutputPath)
};

#endif // CCTWIMPORTDATA_H
