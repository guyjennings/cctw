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

  void                incChunksRead(int n);
  void                incChunksWritten(int n);
  void                incChunksHeld(int n);

public slots:
  void                sizingChanged();
  virtual void        setDataSource(QString desc);
  virtual void        setDims(QString desc);
  virtual void        setChunks(QString desc);
  virtual void        setDataset(QString desc);
  bool                containsPixel(CctwIntVector3D pixelCoord);
  bool                containsChunk(int ix, int iy, int iz);

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

  void                beginTransform(bool isInput, int transformOptions);
  void                endTransform();

  bool                openOutputFile();
  bool                checkInputFile();
  bool                openInputFile(bool quietly = false);
  bool                openInputNeXusFile();
  void                closeOutputFile();
  void                closeInputFile();
  void                closeInputNeXusFile();

private slots:
  void                onDataFileNameChanged();

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

  Q_PROPERTY(CctwIntVector3D dimensions READ get_Dimensions WRITE set_Dimensions)
  CCTW_INTVECTOR3D_PROPERTY(Dimensions)

  Q_PROPERTY(CctwIntVector3D chunkSize READ get_ChunkSize WRITE set_ChunkSize)
  CCTW_INTVECTOR3D_PROPERTY(ChunkSize)

  Q_PROPERTY(CctwIntVector3D chunkCount READ get_ChunkCount WRITE set_ChunkCount STORED false)
  CCTW_INTVECTOR3D_PROPERTY(ChunkCount)

  Q_PROPERTY(int compression READ get_Compression WRITE set_Compression)
  QCEP_INTEGER_PROPERTY(Compression)

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

  static QMutex       m_FileAccessMutex;
};

#endif // CCTWCHUNKEDDATA_H
