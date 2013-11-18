#include "cctwdatablobs.h"

CctwDataBlob::CctwDataBlob(int blobType, int blobID, CctwIntVector3D blobDimensions, int dataLength, int weightLength)
  : m_BlobType(blobType),
    m_BlobID(blobID),
    m_BlobDimensions(blobDimensions),
    m_DataLength(dataLength),
    m_WeightLength(weightLength)
{
}

CctwInputDataBlob::CctwInputDataBlob(int blobID, CctwIntVector3D blobDimensions)
  : CctwDataBlob(0, blobID, blobDimensions, blobDimensions.volume(), blobDimensions.volume())
{
}

CctwIntermediateDataBlob::CctwIntermediateDataBlob(int blobID, CctwIntVector3D blobDimensions)
  : CctwDataBlob(1, blobID, blobDimensions, blobDimensions.volume(), blobDimensions.volume())
{
}

CctwOutputDataBlob::CctwOutputDataBlob(int blobID, CctwIntVector3D blobDimensions)
  : CctwDataBlob(2, blobID, blobDimensions, blobDimensions.volume(), 0)
{
}

int CctwDataBlob::blobID()
{
  return m_BlobID;
}

int CctwDataBlob::dataLength()
{
  return m_DataLength;
}

int CctwDataBlob::weightLength()
{
  return m_WeightLength;
}

int CctwDataBlob::blobLength()
{
  return sizeof(CctwDataBlob) + m_DataLength*sizeof(double) + m_WeightLength*sizeof(double);
}

double *CctwDataBlob::data()
{
  if (m_DataLength) {
    return &m_Data[0];
  } else {
    return NULL;
  }
}

double *CctwDataBlob::weight()
{
  if (m_WeightLength) {
    return &m_Data[m_DataLength];
  } else {
    return NULL;
  }
}

CctwInputDataBlob* CctwInputDataBlob::newInputDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = ::malloc(sizeof(CctwDataBlob)+ size*sizeof(double)*2);

  CctwInputDataBlob *blob = new (buff) CctwInputDataBlob(blobId, blobDimensions);

  return blob;
}

CctwIntermediateDataBlob* CctwIntermediateDataBlob::newIntermediateDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = malloc(sizeof(CctwDataBlob) + size*sizeof(double)*2);

  CctwIntermediateDataBlob *blob = new (buff) CctwIntermediateDataBlob(blobId, blobDimensions);

  return blob;
}

CctwOutputDataBlob* CctwOutputDataBlob::newOutputDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = malloc(sizeof(CctwDataBlob) + size*sizeof(double));

  CctwOutputDataBlob *blob = new (buff) CctwOutputDataBlob(blobId, blobDimensions);

  return blob;
}

void CctwDataBlob::deleteBlob(int blobId, CctwDataBlob *blob)
{
  ::free(blob);
}
