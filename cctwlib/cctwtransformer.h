#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwobject.h"
#include "cctwchunkeddata.h"
#include "cctwtransforminterface.h"
#include <QUrl>

template <class T>
class QcepImageData;

class CctwApplication;

class CctwTransformer : public CctwObject
{
  Q_OBJECT
public:
  CctwTransformer(CctwApplication        *application,
                  CctwChunkedData        *input,    // The input data
                  CctwChunkedData        *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
//                  int osx, int osy, int osz,        // Oversampling factors
                  QString name,
                  QObject *parent);

  virtual ~CctwTransformer();

public slots:
  void transform();
  void checkTransform();

  void saveDependencies(QString path);
  void loadDependencies(QString path);

  QcepIntList dependencies(int n);
  QList<CctwIntVector3D> dependencies(int cx, int cy, int cz);

  void transformChunkNumber(int n);
  void dummyTransformChunkNumber(int n);

  void clearDependencies();
  void addDependency(int f, int t);

  void dummyTransform1();
  void dummyTransform2();
  void dummyTransform3();

  void projectInput(QString path, int axes);
  void projectOutput(QString path, int axes);

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

private:
  void markInputChunkNeeded(CctwIntVector3D idx);
  void runTransformChunkNumber(int n);
  void runDummyTransformChunkNumber(int n);

  void projectDatasetChunk(CctwChunkedData *data, int chunk, int axes);
  void projectDataset(QString path, CctwChunkedData *data, int axes);

private:
  CctwApplication         *m_Application;
  QAtomicInt               m_MergeCounter;
  CctwChunkedData         *m_InputData;
  CctwChunkedData         *m_OutputData;
  CctwTransformInterface  *m_Transform;
//  int                      m_OversampleX;
//  int                      m_OversampleY;
//  int                      m_OversampleZ;

  QMutex                   m_LockX;
  QMutex                   m_LockY;
  QMutex                   m_LockZ;

  QcepImageData<double>   *m_ImageX;
  QcepImageData<double>   *m_ImageY;
  QcepImageData<double>   *m_ImageZ;

public:
  Q_PROPERTY(double wallTime READ get_WallTime WRITE set_WallTime STORED false)
  QCEP_DOUBLE_PROPERTY(WallTime)

  Q_PROPERTY(int blocksLimit READ get_BlocksLimit WRITE set_BlocksLimit)
  QCEP_INTEGER_PROPERTY(BlocksLimit)

  Q_PROPERTY(int transformOptions READ get_TransformOptions WRITE set_TransformOptions)
  QCEP_INTEGER_PROPERTY(TransformOptions)

  Q_PROPERTY(int oversampleX READ get_OversampleX WRITE set_OversampleX)
  QCEP_INTEGER_PROPERTY(OversampleX)

  Q_PROPERTY(int oversampleY READ get_OversampleY WRITE set_OversampleY)
  QCEP_INTEGER_PROPERTY(OversampleY)

  Q_PROPERTY(int oversampleZ READ get_OversampleZ WRITE set_OversampleZ)
  QCEP_INTEGER_PROPERTY(OversampleZ)

  Q_PROPERTY(bool projectX READ get_ProjectX WRITE set_ProjectX)
  QCEP_BOOLEAN_PROPERTY(ProjectX)

  Q_PROPERTY(bool projectY READ get_ProjectY WRITE set_ProjectY)
  QCEP_BOOLEAN_PROPERTY(ProjectY)

  Q_PROPERTY(bool projectZ READ get_ProjectZ WRITE set_ProjectZ)
  QCEP_BOOLEAN_PROPERTY(ProjectZ)

  Q_PROPERTY(QString projectDestination READ get_ProjectDestination WRITE set_ProjectDestination)
  QCEP_STRING_PROPERTY(ProjectDestination)
};

#endif // CCTWTRANSFORMER_H
