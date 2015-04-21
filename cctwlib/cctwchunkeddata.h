#ifndef CCTWCHUNKEDDATA_H
#define CCTWCHUNKEDDATA_H

#include <QVector>
#include "cctwobject.h"
#include "cctwvector3d.h"
#include "cctwintvector3dproperty.h"
#include "qcepproperty.h"
#include "hdf5.h"

#define NEXUS_ENABLED 0
#if NEXUS_ENABLED == 1
#include <nexus/NeXusFile.hpp>
#endif

class CctwDataChunk;
class CctwApplication;
class CctwTransformer;

class CctwChunkedData : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwChunkedData(CctwApplication *application,
                           CctwIntVector3D dim,          // Data set dimension
                           CctwIntVector3D chunkSize,    // Chunk size
                           bool isInput,                 // is it an input
                           QString name,
                           QObject *parent);
  void allocateChunks();

  typedef float MergeDataType;
#define CCTW_H5T_INTERNAL_TYPE H5T_NATIVE_FLOAT

signals:
  void chunkSizeChanged(CctwIntVector3D sz);
  void dimensionsChanged(CctwIntVector3D dm);
  void chunkCountChanged(CctwIntVector3D ct);

public:
  CctwIntVector3D     dimensions() const   { return m_DimensionsCache; }
  CctwIntVector3D     chunkSize() const    { return m_ChunkSizeCache; }

  void                setDimensions(CctwIntVector3D dim);
  void                setChunkSize(CctwIntVector3D cksz);

  void                setMaskDimensions(int mx, int my);
  void                setAnglesDimensions(int n);
  void                setWeightsDimensions(int n);

  void                incChunksRead(int n);
  void                incChunksWritten(int n);
  void                incChunksHeld(int n);

public slots:
  void                sizingChanged();
  virtual void        setDataSource(QString desc);
  virtual void        setDims(QString desc);
  virtual void        setChunks(QString desc);
  virtual void        setDataset(QString desc);
  virtual void        setMaskSource(QString desc);
  virtual void        setMaskDataset(QString desc);
  virtual void        setAnglesSource(QString desc);
  virtual void        setAnglesDataset(QString desc);
  virtual void        setWeightsSource(QString desc);
  virtual void        setWeightsDataset(QString desc);
  bool                containsPixel(CctwIntVector3D pixelCoord);
  bool                containsChunk(int ix, int iy, int iz);
  int                 allocatedChunkCount();

  void                setAngle(int n, double v);
  double              angle(int n);

  void                setWeight(int n, double v);
  double              weight(int n);

  CctwIntVector3D     chunkStart(int n);    // Return pixel coords of start of chunk chunkIdx
  int                 chunkContaining(CctwIntVector3D pixelCoord);  // Return index of chunk containing given pixel
  int                 chunkContaining(CctwDoubleVector3D fracPixelCoord);  // Return index of chunk containing fractional pixel coords
  CctwIntVector3D     chunkCount() const;

  CctwIntVector3D     chunkIndexFromNumber(int n);
  int                 chunkNumberFromIndex(CctwIntVector3D chunkIdx);

  void                clearDependencies();
  void                addDependency(int f, int t);

  CctwDataChunk      *chunk(int n);
  CctwDataChunk      *chunk(CctwIntVector3D idx);

  CctwDataChunk      *readChunk(int n);
  void                writeChunk(int n);
  void                releaseChunk(int n);
  void                releaseChunkData(int n);
  void                normalizeChunk(int n);
  void                mergeChunk(CctwDataChunk *chunk);
  void                clearMergeCounters();

  bool                beginTransform(bool isInput, int transformOptions);
  void                endTransform();

  bool                openOutputFile();
  bool                checkInputFile();
  bool                openInputFile(bool quietly = false);
  bool                openInputNeXusFile();
  void                flushOutputFile();
  void                closeOutputFile();
  void                closeInputFile(bool quietly = false);
  void                closeInputNeXusFile();

  bool                checkMaskFile();
  bool                openMaskFile(bool quietly = false);
  void                closeMaskFile(bool quietly = false);
  bool                readMaskFile();

  bool                checkAnglesFile();
  bool                openAnglesFile(bool quietly = false);
  void                closeAnglesFile(bool quietly = false);
  bool                readAnglesFile();

  bool                checkWeightsFile();
  bool                openWeightsFile(bool quietly = false);
  void                closeWeightsFile(bool quietly = false);
  bool                readWeightsFile();

private slots:
  void                onDataFileNameChanged();
  void                onMaskFileNameChanged();
  void                onAnglesChanged();

protected:
  QVector< CctwDataChunk* >  m_DataChunks;
  /** Is this a NeXus file? */
  bool                 m_IsNeXus;
#if NEXUS_ENABLED == 1
  NeXus::File          *m_NeXusFile;
#endif

private:
  QMutex              m_ChunkLock;
  CctwIntVector3D     m_DimensionsCache;
  CctwIntVector3D     m_ChunkSizeCache;
  CctwIntVector3D     m_ChunkCountCache;

  Q_PROPERTY(QString dataFileName READ get_DataFileName WRITE set_DataFileName)
  QCEP_STRING_PROPERTY(DataFileName)

  Q_PROPERTY(QString dataSetName READ get_DataSetName WRITE set_DataSetName)
  QCEP_STRING_PROPERTY(DataSetName)

  Q_PROPERTY(QString maskDataFileName READ get_MaskDataFileName WRITE set_MaskDataFileName)
  QCEP_STRING_PROPERTY(MaskDataFileName)

  Q_PROPERTY(QString maskDataSetName READ get_MaskDataSetName WRITE set_MaskDataSetName)
  QCEP_STRING_PROPERTY(MaskDataSetName)

  Q_PROPERTY(QcepIntVector mask READ get_Mask WRITE set_Mask STORED false)
  QCEP_INTEGER_VECTOR_PROPERTY(Mask)

  Q_PROPERTY(QString anglesDataFileName READ get_AnglesDataFileName WRITE set_AnglesDataFileName)
  QCEP_STRING_PROPERTY(AnglesDataFileName)

  Q_PROPERTY(QString anglesDataSetName READ get_AnglesDataSetName WRITE set_AnglesDataSetName)
  QCEP_STRING_PROPERTY(AnglesDataSetName)

  Q_PROPERTY(QcepDoubleVector angles READ get_Angles WRITE set_Angles STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Angles)

  Q_PROPERTY(QString weightsDataFileName READ get_WeightsDataFileName WRITE set_WeightsDataFileName)
  QCEP_STRING_PROPERTY(WeightsDataFileName)

  Q_PROPERTY(QString weightsDataSetName READ get_WeightsDataSetName WRITE set_WeightsDataSetName)
  QCEP_STRING_PROPERTY(WeightsDataSetName)

  Q_PROPERTY(QcepDoubleVector weights READ get_Weights WRITE set_Weights STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(Weights)

  Q_PROPERTY(CctwIntVector3D dimensions READ get_Dimensions WRITE set_Dimensions)
  CCTW_INTVECTOR3D_PROPERTY(Dimensions)

  Q_PROPERTY(CctwIntVector3D chunkSize READ get_ChunkSize WRITE set_ChunkSize)
  CCTW_INTVECTOR3D_PROPERTY(ChunkSize)

  Q_PROPERTY(CctwIntVector3D chunkCount READ get_ChunkCount WRITE set_ChunkCount STORED false)
  CCTW_INTVECTOR3D_PROPERTY(ChunkCount)

  Q_PROPERTY(int compression READ get_Compression WRITE set_Compression)
  QCEP_INTEGER_PROPERTY(Compression)

  Q_PROPERTY(int normalization READ get_Normalization WRITE set_Normalization)
  QCEP_INTEGER_PROPERTY(Normalization)

  Q_PROPERTY(CctwIntVector3D hdfChunkSize READ get_HDFChunkSize WRITE set_HDFChunkSize)
  CCTW_INTVECTOR3D_PROPERTY(HDFChunkSize)

  Q_PROPERTY(int chunksRead READ get_ChunksRead WRITE set_ChunksRead STORED false)
  QCEP_INTEGER_PROPERTY(ChunksRead)

  Q_PROPERTY(int chunksWritten READ get_ChunksWritten WRITE set_ChunksWritten STORED false)
  QCEP_INTEGER_PROPERTY(ChunksWritten)

  Q_PROPERTY(int chunksHeld READ get_ChunksHeld WRITE set_ChunksHeld STORED false)
  QCEP_INTEGER_PROPERTY(ChunksHeld)

  Q_PROPERTY(int chunksHeldMax READ get_ChunksHeldMax WRITE set_ChunksHeldMax STORED false)
  QCEP_INTEGER_PROPERTY(ChunksHeldMax)

  bool                m_IsInput;
  int                 m_TransformOptions;
  hid_t               m_FileId;
  hid_t               m_DatasetId;
  hid_t               m_DataspaceId;
  hid_t               m_Dataset2Id;
  hid_t               m_Dataspace2Id;

  bool                m_MaskSameFile; // If mask is in input data file
  hid_t               m_MaskFileId;
  hid_t               m_MaskDatasetId;
  hid_t               m_MaskDataspaceId;

  bool                m_AnglesSameFile; // If angles are in input data file
  hid_t               m_AnglesFileId;
  hid_t               m_AnglesDatasetId;
  hid_t               m_AnglesDataspaceId;

  bool                m_WeightsSameFile; // If weights are in input data file
  hid_t               m_WeightsFileId;
  hid_t               m_WeightsDatasetId;
  hid_t               m_WeightsDataspaceId;

  static QMutex       m_FileAccessMutex;
};

#endif // CCTWCHUNKEDDATA_H
