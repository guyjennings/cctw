#include "cctwtransformer.h"
#include "cctwcommandline.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cctwmatrix3x3.h"

CctwTransformer::CctwTransformer(CctwInputDataInterface *input,
                                 CctwOutputDataInterface *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz, int nTests)
  : m_InputData(input),
    m_OutputData(output),
    m_Transform(xform),
    m_OversampleX(osx),
    m_OversampleY(osy),
    m_OversampleZ(osz),
    m_Tests(nTests)
{
}

CctwTransformer *CctwTransformer::createNew(int argc, char *argv[],
                                           CctwInputDataInterface *input,    // The input data
                                           CctwOutputDataInterface *output,  // The output data
                                           CctwTransformInterface *xform)
{
  int osx = CctwCommandLine::integerValue(argc, argv, "--osx", 1);
  int osy = CctwCommandLine::integerValue(argc, argv, "--osy", 1);
  int osz = CctwCommandLine::integerValue(argc, argv, "--osz", 1);

  int nTests = CctwCommandLine::integerValue(argc, argv, "--test", 0);

  CctwTransformer *result = new CctwTransformer(input, output, xform, osx, osy, osz, nTests);

  if (nTests && result) {
    result->performTests();
  }

  return result;
}

void CctwTransformer::transformChunk(int nx, int ny, int nz)
{
  CctwIntVector3D start = m_OutputData -> chunkStart(CctwIntVector3D(nx,ny,nz));
  CctwIntVector3D end   = m_OutputData -> chunkStart(CctwIntVector3D(nx+1,ny+1,nz+1));

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

void CctwTransformer::performTests()
{
  printf("Performing tests\n");

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

      printf("Iter %d.1: Det %g, Diag %g, Upper %g, Lower %g\n", i, det, sum1, sum2 ,sum3);
    }
  }
}
