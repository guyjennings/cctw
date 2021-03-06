#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwobject.h"
#include "cctwchunkeddata-ptr.h"
#include "cctwdatachunk-ptr.h"
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
                  CctwChunkedDataPtr     input,    // The input data
                  CctwChunkedDataPtr     output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  QString                name);

  virtual ~CctwTransformer();

public slots:
  void transform(int mode=0);
//  void checkTransform();

  void runTransformChunkNumber(int n, int mode);
  void transformChunkNumber(int chunkId, int mode);
  void transformChunkData(int chunkId, int mode,
                          CctwDataChunk *inputChunk,
                          QMap<int, CctwDataChunk*> &outputChunks);

#ifdef WANT_ANALYSIS_COMMANDS
  void inputProject(QString path, int axes);
  void outputProject(QString path, int axes);
#endif

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

private:
  void markInputChunkNeeded(CctwIntVector3D idx);

#ifdef WANT_ANALYSIS_COMMANDS
  void projectDatasetChunk(CctwChunkedDataPtr data, int chunk, int axes);
  void projectDataset(QString path, CctwChunkedDataPtr data, int axes);
#endif

  bool parseSubset(CctwChunkedDataPtr data);

private:
  CctwApplication         *m_Application;
  CctwChunkedDataPtr       m_InputData;
  CctwChunkedDataPtr       m_OutputData;
  CctwTransformInterface  *m_Transform;

  QMutex                   m_LockX;
  QMutex                   m_LockY;
  QMutex                   m_LockZ;

  QcepImageData<double>   *m_ImageX;
  QcepImageData<double>   *m_ImageY;
  QcepImageData<double>   *m_ImageZ;
  QcepImageData<double>   *m_WeightX;
  QcepImageData<double>   *m_WeightY;
  QcepImageData<double>   *m_WeightZ;

  double m_MinData;
  double m_MaxData;
  double m_MinWeight;
  double m_MaxWeight;

  CctwIntVector3D m_SubsetStart;
  CctwIntVector3D m_SubsetEnd;

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

  Q_PROPERTY(int normalization READ get_Normalization WRITE set_Normalization)
  QCEP_INTEGER_PROPERTY(Normalization)

  Q_PROPERTY(int compression READ get_Compression WRITE set_Compression)
  QCEP_INTEGER_PROPERTY(Compression)

  Q_PROPERTY(QString subset READ get_Subset WRITE set_Subset STORED false)
  QCEP_STRING_PROPERTY(Subset)

  Q_PROPERTY(int skipped READ get_Skipped WRITE set_Skipped STORED false)
  QCEP_INTEGER_PROPERTY(Skipped)
};

#endif // CCTWTRANSFORMER_H
