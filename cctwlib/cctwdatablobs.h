#ifndef CCTWDATABLOBS_H
#define CCTWDATABLOBS_H

#include "cctwvector3d.h"

class CctwDataBlob
{
protected:
  CctwDataBlob(int blobType, int blobID, CctwIntVector3D blobDimensions, int dataLength, int weightLength);

public:
  static void deleteBlob(int blobId, CctwDataBlob *blob);

  int blobType();
  int blobID();
  int blobLength();
  int dataLength();
  int weightLength();
  int offset(int x, int y, int z);
  int offset(CctwIntVector3D c);

  double *data();
  double *weight();

  double& data(int offset);
  double& weight(int offset);

private:
  int             m_BlobType;
  int             m_BlobID;
  CctwIntVector3D m_BlobDimensions;
  int             m_DataLength;
  int             m_WeightLength;
  double          m_Data[1];
};

class CctwInputDataBlob : public CctwDataBlob
{
protected:
  CctwInputDataBlob(int blobID, CctwIntVector3D blobDimensions);

public:
  static CctwInputDataBlob*        newInputDataBlob       (int blobId, CctwIntVector3D blobDimensions);
};

class CctwOutputDataBlob : public CctwDataBlob
{
protected:
  CctwOutputDataBlob(int blobID, CctwIntVector3D blobDimensions);

public:
  static CctwOutputDataBlob*       newOutputDataBlob      (int blobId, CctwIntVector3D blobDimensions);
};

class CctwIntermediateDataBlob : public CctwDataBlob
{
protected:
  CctwIntermediateDataBlob(int blobID, CctwIntVector3D blobDimensions);

public:
  static CctwIntermediateDataBlob* newIntermediateDataBlob(int blobId, CctwIntVector3D blobDimensions);
};



#endif // CCTWDATABLOBS_H
