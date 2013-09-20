#ifndef CCTWDATACHUNK_H
#define CCTWDATACHUNK_H

#include "cctwvector3d.h"
#include "cctwchunkindex.h"
class CctwChunkedDataInterface;

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#ifdef USE_QT
class CctwDataChunk : CctwqtObject
{
  Q_OBJECT
public:
  CctwDataChunk(CctwChunkedDataInterface *data, CctwChunkIndex index, QObject *parent);
#else
class CctwDataChunk
{
public:
  CctwDataChunk(CctwChunkedDataInterface *data, CctwChunkIndex index);
#endif

  virtual ~CctwDataChunk();

#ifdef USE_QT
public slots:
#else
public:
#endif

  int allocateData();
  int allocateWeights();
  int deallocateData();
  int deallocateWeights();

  double data(CctwIntVector3D localcoords);
  double weight(CctwIntVector3D localcoords);
  void setData(CctwIntVector3D localcoords, double val);
  void setWeight(CctwIntVector3D localcoords, double val);
  int pixelOffset(CctwIntVector3D localcoords);

private:
  CctwChunkedDataInterface *m_Data;
  CctwChunkIndex            m_ChunkIndex;
  double                   *m_ChunkData;
  double                   *m_ChunkWeights;
};

#endif // CCTWDATACHUNK_H
