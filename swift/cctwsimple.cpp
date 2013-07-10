
#include <stdio.h>

#include <cctwchunkeddatainterface.h>
#include <cctwcrystalcoordinatetransform.h>
#include <cctwtransformer.h>
#include <cctwtransforminterface.h>

#include <swift/cctwsimple.h>

static bool chunkFromBlob(const void *blob,
                          CctwChunkedDataInterface *chunk)
{
  return true;
}

int CctwMaxRequisites(void)
{
  return CctwTransformer::MAX_REQUISITES;
}

void CctwSimpleTransform(int osx, int osy, int osz,
                         void **inputBlobs, int inputCount,
                         void **outputBlobs)
{
  CctwChunkedDataInterface inputChunks[inputCount];

  for (int i = 0; i < inputCount; i++)
    chunkFromBlob(inputBlobs[i], &inputChunks[i]);

//  CctwInputDataInterface inputInterface;
//
//  CctwTransformInterface *xform =
//      new CctwCrystalCoordinateTransform();
//  CctwTransformer *transformer =
//      new CctwTransformer(input, output, xform, osx, osy, osz);
//
//  tranformer->createNew(inputChunks, inputCount);

}
