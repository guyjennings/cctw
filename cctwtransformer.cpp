#include "cctwtransformer.h"
#include "cctwcommandline.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cctwmatrix3x3.h"

#ifdef USE_QT
CctwTransformer::CctwTransformer(CctwInputDataInterface *input,
                                 CctwOutputDataInterface *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz, int nTests, QObject *parent) :
  CctwqtObject(parent),
#else
CctwTransformer::CctwTransformer(CctwInputDataInterface *input,
                                 CctwOutputDataInterface *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz, int nTests) :
#endif
    m_InputData(input),
    m_OutputData(output),
    m_Transform(xform),
    m_OversampleX(osx),
    m_OversampleY(osy),
    m_OversampleZ(osz),
    m_Tests(nTests),
    m_ChunkCount(input->chunkCount()),
    m_ChunksTotal(m_ChunkCount.x()*m_ChunkCount.y()*m_ChunkCount.z()),
    m_ChunksUsed(NULL)
{
  m_ChunksUsed = new int[m_ChunksTotal];

  printf("CctwTransformer::CctwTransformer - Chunk Count : [%d,%d,%d], total %d\n",
         m_ChunkCount.x(), m_ChunkCount.y(), m_ChunkCount.z(), m_ChunksTotal);
}

CctwTransformer::~CctwTransformer()
{
  delete [] m_ChunksUsed;
}

//CctwTransformer *CctwTransformer::createNew(int argc, char *argv[],
//                                           CctwInputDataInterface *input,    // The input data
//                                           CctwOutputDataInterface *output,  // The output data
//                                           CctwTransformInterface *xform)
//{
//  int osx = CctwCommandLine::integerValue(argc, argv, "--osx", 1);
//  int osy = CctwCommandLine::integerValue(argc, argv, "--osy", 1);
//  int osz = CctwCommandLine::integerValue(argc, argv, "--osz", 1);

//  int nTests = CctwCommandLine::integerValue(argc, argv, "--test", 0);

//  CctwTransformer *result = new CctwTransformer(input, output, xform, osx, osy, osz, nTests);

//  if (nTests && result) {
//    result->performTests();
//  }

//  return result;
//}

void CctwTransformer::transformChunk(int nx, int ny, int nz)
{
  static int logCount = 0;

  CctwDoubleVector3D inLow = m_InputData->toReal(CctwIntVector3D(0,0,0));
  CctwDoubleVector3D inHigh = m_InputData->toReal(m_InputData->dimensions());

  printf("Input Data range [%g,%g,%g] to [%g,%g,%g]\n",
         inLow.x(), inLow.y(), inLow.z(),
         inHigh.x(), inHigh.y(), inHigh.z());

  CctwDoubleVector3D outLow = m_OutputData->toReal(CctwIntVector3D(0,0,0));
  CctwDoubleVector3D outHigh = m_OutputData->toReal(m_OutputData->dimensions());

  printf("Output Data range [%g,%g,%g] to [%g,%g,%g]\n",
         outLow.x(), outLow.y(), outLow.z(),
         outHigh.x(), outHigh.y(), outHigh.z());

  CctwIntVector3D start = m_OutputData -> chunkStart(CctwIntVector3D(nx,ny,nz));
  CctwIntVector3D end   = m_OutputData -> chunkStart(CctwIntVector3D(nx+1,ny+1,nz+1));

  CctwDoubleVector3D startReal = m_OutputData->toReal(start);
  CctwDoubleVector3D endReal   = m_OutputData->toReal(end);

  printf("Transformed data range [%g,%g,%g] to [%g,%g,%g]\n",
         startReal.x(), startReal.y(), startReal.z(),
         endReal.x(), endReal.y(), endReal.z());

  bool first = true;
  CctwDoubleVector3D min, max;

  for (int iz = start.z(); iz < end.z(); iz++) {
    for (int iy = start.y(); iy < end.y(); iy++) {
      for (int ix = start.x(); ix < end.x(); ix++) {
        CctwDoubleVector3D real = m_OutputData->toReal(CctwIntVector3D(ix,iy,iz));
        CctwDoubleVector3D inv = m_Transform->inverse(real);

        if (first) {
          first = false;
          min = inv;
          max = min;
        } else {
          min = min.min(inv);
          max = max.max(inv);
        }

        CctwIntVector3D realp = m_OutputData->toPixel(real);
        CctwIntVector3D invp = m_InputData->toPixel(inv);

        CctwIntVector3D chk = m_InputData->chunkIndex(invp);

        if (logCount++ < 100) {
          printf("%3d: [%g,%g,%g] -> [%g,%g,%g] == ", logCount, real.x(), real.y(), real.z(), inv.x(), inv.y(), inv.z());
          printf("%3d: [%d,%d,%d] -> [%d,%d,%d] -> [%d,%d,%d]\n", logCount, realp.x(), realp.y(), realp.z(), invp.x(), invp.y(), invp.z(),
                 chk.x(), chk.y(), chk.z());
        }

        markInputChunkNeeded(chk);
      }
    }
  }

  CctwIntVector3D mininp = m_InputData->toPixel(min);
  CctwIntVector3D maxinp = m_InputData->toPixel(max);

  printf("Range of input data for chunk [%d,%d,%d]\n", nx,ny,nz);

  printf("Output Pixel Range\n");
  printf("X Pixel Min: %d Max %d\n", start.x(), end.x());
  printf("Y Pixel Min: %d Max %d\n", start.y(), end.y());
  printf("Z Pixel Min: %d Max %d\n", start.z(), end.z());

  printf("Input Pixel Range\n");
  printf("X Pixel Min: %d Max %d\n", mininp.x(), maxinp.x());
  printf("Y Pixel Min: %d Max %d\n", mininp.y(), maxinp.y());
  printf("Z Pixel Min: %d Max %d\n", mininp.z(), maxinp.z());

  printf("Input Coord Range\n");
  printf("X Min: %g Max: %g\n", min.x(), max.x());
  printf("Y Min: %g Max: %g\n", min.y(), max.y());
  printf("Z Min: %g Max: %g\n", min.z(), max.z());


}

void CctwTransformer::markInputChunkNeeded(CctwIntVector3D idx)
{
  static int errCount = 0;
  static int chnkCount = 0;

  if (idx.x() >= 0 && idx.x() < m_ChunkCount.x() &&
      idx.y() >= 0 && idx.y() < m_ChunkCount.y() &&
      idx.z() >= 0 && idx.z() < m_ChunkCount.z()) {
    int n = XYZtoID(m_ChunkCount.x(), m_ChunkCount.y(), m_ChunkCount.z(),
                    idx.x(), idx.y(), idx.z());

    if (n >= 0 && n < m_ChunksTotal) {
      if (m_ChunksUsed) {
        if (m_ChunksUsed[n] == false) {
          m_ChunksUsed[n] = true;

          if (chnkCount++ < 100) {
            printf("Chunk %d used\n", n);
          }
        }
      }
    } else {
      if (errCount++ < 100) {
        printf("n (%d) out of range [0..%d)\n", n, m_ChunksTotal);
      }
    }
  } else {
    if (errCount++ < 100) {
      printf("idx (%d,%d,%d) out of range [[0,0,0]..[%d,%d,%d])\n",
             idx.x(), idx.y(), idx.z(), m_ChunkCount.x(), m_ChunkCount.y(), m_ChunkCount.z());
    }
  }
}

int CctwTransformer::XYZtoID(int max_x, int max_y, int max_z,
                             int x, int y, int z)
{
  return x*max_y*max_z + y*max_z + z;
}

void CctwTransformer::performTests()
{
  printf("Performing tests\n");

#ifndef WIN32
  for (int i=0; i<m_Tests; i++) {
    {
      double rot1 = 2.0*M_PI*(double)random()/(double)RAND_MAX;
      double rot2 = 2.0*M_PI*(double)random()/(double)RAND_MAX;
      double rot3 = 2.0*M_PI*(double)random()/(double)RAND_MAX;

      bool invertible;

      CctwDoubleMatrix3x3 m1 = CctwDoubleMatrix3x3::rotationMatrix(rot1, rot2, rot3);
      CctwDoubleMatrix3x3 m2 = m1.inverted(&invertible);
      CctwDoubleMatrix3x3 m3 = m1*m2;

      double det = m3.determinant();

      double sum1 = fabs(m3(0,0)) + fabs(m3(1,1)) + fabs(m3(2,2));
      double sum2 = fabs(m3(0,1)) + fabs(m3(0,2)) + fabs(m3(1,2));
      double sum3 = fabs(m3(1,0)) + fabs(m3(2,0)) + fabs(m3(2,1));

      printf("Iter %d.0: Det %g, Diag %g, Upper %g, Lower %g\n", i, det, sum1, sum2 ,sum3);

      double x = (double)random();
      double y = (double)random();
      double z = (double)random();

      CctwDoubleVector3D v1(x,y,z);
      CctwDoubleVector3D v2 = m1*v1;
      CctwDoubleVector3D v3 = m2*v2;

      double sum4 = (v1 - v3).length();

      printf("Iter %d.1: Len %g\n", i, sum4);
    }

    {
      double a = (double)random();
      double b = (double)random();
      double c = (double)random();
      double d = (double)random();
      double e = (double)random();
      double f = (double)random();
      double g = (double)random();
      double h = (double)random();
      double k = (double)random();

      bool invertible;

      CctwDoubleMatrix3x3 m1(a,b,c,d,e,f,g,h,k);
      CctwDoubleMatrix3x3 m2 = m1.inverted(&invertible);
      CctwDoubleMatrix3x3 m3 = m1*m2;

      double det = m3.determinant();

      double sum1 = fabs(m3(0,0)) + fabs(m3(1,1)) + fabs(m3(2,2));
      double sum2 = fabs(m3(0,1)) + fabs(m3(0,2)) + fabs(m3(1,2));
      double sum3 = fabs(m3(1,0)) + fabs(m3(2,0)) + fabs(m3(2,1));

      printf("Iter %d.2: Det %g, Diag %g, Upper %g, Lower %g\n", i, det, sum1, sum2 ,sum3);
    }
  }
#endif
}
