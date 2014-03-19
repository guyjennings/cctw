#ifndef CCTWCOMPARER_H
#define CCTWCOMPARER_H

#include "cctwobject.h"

class CctwApplication;

class CctwComparer : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwComparer(CctwApplication *application, QString name, QObject *parent = 0);
  virtual ~CctwComparer();

signals:

public slots:
  void compareDatasets();
  void compareDatasetsRigorously();
  void compareDatasetsApproximately();

private:
  CctwApplication *m_Application;

private:
  Q_PROPERTY(QString filePath1 READ get_FilePath1 WRITE set_FilePath1)
  QCEP_STRING_PROPERTY(FilePath1)

  Q_PROPERTY(QString dataset1 READ get_Dataset1 WRITE set_Dataset1)
  QCEP_STRING_PROPERTY(Dataset1)

  Q_PROPERTY(QString filePath2 READ get_FilePath2 WRITE set_FilePath2)
  QCEP_STRING_PROPERTY(FilePath2)

  Q_PROPERTY(QString dataset2 READ get_Dataset2 WRITE set_Dataset2)
  QCEP_STRING_PROPERTY(Dataset2)

  Q_PROPERTY(bool compareRigorously READ get_CompareRigorously WRITE set_CompareRigorously)
  QCEP_BOOLEAN_PROPERTY(CompareRigorously)

  Q_PROPERTY(bool compareApproximately READ get_CompareApproximately WRITE set_CompareApproximately)
  QCEP_BOOLEAN_PROPERTY(CompareApproximately)
};

#endif // CCTWCOMPARER_H
