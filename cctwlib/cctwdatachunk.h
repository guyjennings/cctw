#ifndef CCTWDATACHUNK_H
#define CCTWDATACHUNK_H

#include "cctwvector3d.h"
//#include "cctwchunkindex.h"
class CctwChunkedDataInterface;

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#ifdef USE_QT
class CctwDataChunk : public CctwqtObject
{
  Q_OBJECT
public:
  CctwDataChunk(CctwChunkedDataInterface *data, CctwIntVector3D index, QObject *parent);
#else
class CctwDataChunk
{
public:
  CctwDataChunk(CctwChunkedDataInterface *data, CctwIntVector3D index);
#endif

  virtual ~CctwDataChunk();

#ifdef USE_QT
public slots:
#else
public:
#endif

  virtual int readData();
  virtual int readWeights();
  virtual int normalize();
  virtual int writeData();
  virtual int writeWeights();

  int allocateData();
  int allocateWeights();
  int deallocateData();
  int deallocateWeights();

  bool dataAllocated() const;
  bool weightsAllocated() const;

  double data(CctwIntVector3D localcoords);
  double weight(CctwIntVector3D localcoords);
  void setData(CctwIntVector3D localcoords, double val);
  void setWeight(CctwIntVector3D localcoords, double val);
  int pixelOffset(CctwIntVector3D localcoords);

  double *dataPointer();
  double *weightsPointer();

  CctwIntVector3D chunkSize();

protected:
  virtual double *allocateBuffer() = 0;
  virtual void releaseBuffer(double *) = 0;

protected:
  CctwChunkedDataInterface *m_Data;
  CctwIntVector3D           m_ChunkIndex;
  double                   *m_ChunkData;
  double                   *m_ChunkWeights;
  int                       m_Normalized;
  int                       m_DataWritten;
  int                       m_WeightsWritten;
};

#endif // CCTWDATACHUNK_H