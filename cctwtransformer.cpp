#include "cctwtransformer.h"
#include "cctwcommandline.h"
#include <stdio.h>

CctwTransformer::CctwTransformer(CctwInputDataInterface *input,
                                 CctwOutputDataInterface *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz)
  : m_InputData(input),
    m_OutputData(output),
    m_Transform(xform),
    m_OversampleX(osx),
    m_OversampleY(osy),
    m_OversampleZ(osz)
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

  CctwTransformer *result = new CctwTransformer(input, output, xform, osx, osy, osz);

  return result;
}

void CctwTransformer::transformChunk(int nx, int ny, int nz)
{
  CctwIntVector3D dim  = m_OutputData->dimensions();
  CctwIntVector3D chnk = m_OutputData->chunkSize();
  CctwDoubleVector3D orig = m_OutputData->origin();
  CctwDoubleVector3D scal = m_OutputData->scale();

  CctwIntVector3D start = chnk*CctwIntVector3D(nx,ny,nz);
  CctwIntVector3D end   = start+chnk;

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
