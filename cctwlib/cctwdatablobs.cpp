#include "cctwdatablobs.h"
#include <QMap>
#include <stdio.h>

static QMap<long, CctwDataBlob*>             g_Blobs;
static QMap<long, CctwInputDataBlob*>        g_InputBlobs;
static QMap<long, CctwIntermediateDataBlob*> g_IntermediateBlobs;
static QMap<long, CctwOutputDataBlob*>       g_OutputBlobs;

CctwDataBlob::CctwDataBlob(int blobType, int blobID, CctwIntVector3D blobDimensions, int dataLength, int weightLength)
  : m_BlobType(blobType),
    m_BlobID(blobID),
    m_BlobDimensions(blobDimensions),
    m_DataLength(dataLength),
    m_WeightLength(weightLength)
{
  double *d = data();
  double *w = weight();

  if (d && dataLength) {
    for (int i = 0; i<dataLength; i++) {
      d[i] = 0;
    }
  }

  if (w && weightLength) {
    for (int i = 0; i<weightLength; i++) {
      w[i] = 0;
    }
  }
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

int CctwDataBlob::blobType()
{
  return m_BlobType;
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

int CctwDataBlob::offset(int x, int y, int z)
{
  int offset = (z * m_BlobDimensions.y() + y)*m_BlobDimensions.x() + x;

  return offset;
}

int CctwDataBlob::offset(CctwIntVector3D c)
{
  return offset(c.x(), c.y(), c.z());
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

double& CctwDataBlob::data(int offset)
{
  if (m_DataLength) {
    return m_Data[offset];
  } else {
    return m_Data[0];
  }
}

double& CctwDataBlob::weight(int offset)
{
  if (m_WeightLength) {
    return m_Data[m_DataLength+offset];
  } else {
    return m_Data[0];
  }
}

CctwInputDataBlob* CctwInputDataBlob::newInputDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = ::malloc(sizeof(CctwDataBlob)+ size*sizeof(double)*2);

  CctwInputDataBlob *blob = new (buff) CctwInputDataBlob(blobId, blobDimensions);
  long blobP = (long) blob;

  if (g_Blobs.contains(blobP)) {
    printf("Duplicate Blobs at %ld\n", blobP);
  } else {
    g_Blobs.insert((long) blob, blob);
  }

  if (g_InputBlobs.contains(blobP)) {
    printf("Duplicate Input Blobs at %ld\n", blobP);
  } else {
    g_InputBlobs.insert((long) blob, blob);
  }

  return blob;
}

CctwIntermediateDataBlob* CctwIntermediateDataBlob::newIntermediateDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = malloc(sizeof(CctwDataBlob) + size*sizeof(double)*2);

  CctwIntermediateDataBlob *blob = new (buff) CctwIntermediateDataBlob(blobId, blobDimensions);
  long blobP = (long) blob;

  if (g_Blobs.contains(blobP)) {
    printf("Duplicate Blobs at %ld\n", blobP);
  } else {
    g_Blobs.insert((long) blob, blob);
  }

  if (g_IntermediateBlobs.contains(blobP)) {
    printf("Duplicate Intermediate Blobs at %ld\n", blobP);
  } else {
    g_IntermediateBlobs.insert((long) blob, blob);
  }

  return blob;
}

CctwOutputDataBlob* CctwOutputDataBlob::newOutputDataBlob(int blobId, CctwIntVector3D blobDimensions)
{
  int size = blobDimensions.volume();

  void *buff = malloc(sizeof(CctwDataBlob) + size*sizeof(double));

  CctwOutputDataBlob *blob = new (buff) CctwOutputDataBlob(blobId, blobDimensions);
  long blobP = (long) blob;

  if (g_Blobs.contains(blobP)) {
    printf("Duplicate Blobs at %ld\n", blobP);
  } else {
    g_Blobs.insert((long) blob, blob);
  }

  if (g_OutputBlobs.contains(blobP)) {
    printf("Duplicate Output Blobs at %ld\n", blobP);
  } else {
    g_OutputBlobs.insert((long) blob, blob);
  }

  return blob;
}

void CctwDataBlob::deleteBlob(int blobId, CctwDataBlob *blob)
{
  long blobP = (long) blob;

  if (CctwDataBlob::validate(blobId, blobP)) {
    int n = g_Blobs.remove(blobP);
    int n1= 0;

    switch (blob->blobType()) {
    case 0:
      n1 = g_InputBlobs.remove(blobP);
      break;
    case 1:
      n1 = g_IntermediateBlobs.remove(blobP);
      break;
    case 2:
      n1 = g_OutputBlobs.remove(blobP);
      break;
    }

    if ((n == 1) && (n1 == 1)) {
      ::free(blob);
    } else {
      printf("Data Blob Validation failed on delete %ld, %d, %d\n",
             blobP, n1, n1);
    }
  }
}

CctwDataBlob* CctwDataBlob::validate(int blobId, long blobP)
{
  if (g_Blobs.contains(blobP)) {
    CctwDataBlob* b = g_Blobs[blobP];
    if (b && (b->blobID() == blobId)) {
      return b;
    } else {
      printf("Blob ID mismatch %d != %d\n", b->blobID(), blobId);
    }
  } else {
    printf("Not a valid blob\n");
  }

  printf("Data Blob Validation failed for %ld\n", blobP);

  return NULL;
}

CctwInputDataBlob* CctwInputDataBlob::validate(int blobId, long blobP)
{
  if (g_Blobs.contains(blobP)) {
    if (g_InputBlobs.contains(blobP)) {
      CctwInputDataBlob *b = g_InputBlobs[blobP];

      if (b && (b->blobID() == blobId) && (b->blobType() == 0)) {
        return b;
      } else {
        printf("Blob ID or blob type mismatch id: %d != %d, type: %d != 0\n", b->blobID(), blobId, b->blobType());
      }
    } else {
      printf("Not a valid input blob\n");
    }
  } else {
    printf("Not a valid blob\n");
  }

  printf("Input Data Blob Validation failed for %ld\n", blobP);

  return NULL;
}

CctwIntermediateDataBlob* CctwIntermediateDataBlob::validate(int blobId, long blobP)
{
  if (g_Blobs.contains(blobP)) {
    if (g_IntermediateBlobs.contains(blobP)) {
      CctwIntermediateDataBlob *b = g_IntermediateBlobs[blobP];

      if (b && (b->blobID() == blobId) && (b->blobType() == 1)) {
        return b;
      } else {
        printf("Blob ID or blob type mismatch id: %d != %d, type: %d != 1\n", b->blobID(), blobId, b->blobType());
      }
    } else {
      printf("Not a valid intermediate blob\n");
    }
  } else {
    printf("Not a valid blob\n");
  }

  printf("Input Data Blob Validation failed for %ld\n", blobP);

  return NULL;
}

CctwOutputDataBlob* CctwOutputDataBlob::validate(int blobId, long blobP)
{
  if (g_Blobs.contains(blobP)) {
    if (g_OutputBlobs.contains(blobP)) {
      CctwOutputDataBlob *b = g_OutputBlobs[blobP];

      if (b && (b->blobID() == blobId) && (b->blobType() == 2)) {
        return b;
      } else {
        printf("Blob ID or blob type mismatch id: %d != %d, type: %d != 2\n", b->blobID(), blobId, b->blobType());
      }
    } else {
      printf("Not a valid output blob\n");
    }
  } else {
    printf("Not a valid blob\n");
  }

  printf("Input Data Blob Validation failed for %ld\n", blobP);

  return NULL;
}
