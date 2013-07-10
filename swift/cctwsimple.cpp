
#include <stdio.h>
#include <stdlib.h>

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

int CctwRequisites(int max_x, int max_y, int max_z,
                   int osx, int osy, int osz, int* output)
{
  /*
   TODO: rough sketch here...
   CctwTransformInterface *xform =
     new CctwCrystalCoordinateTransform();
   Set S; (unique entries)
   for each sample in block x,y,z
     S += xform.inverse(sample);
   put IDs in S into output
   return size(S)
    */
  int count = rand() % 4;

  for (int i = 0; i < count; i++)
  {
    int x = rand() % max_x;
    int y = rand() % max_y;
    int z = rand() % max_z;
    int id = CctwTransformer::XYZtoID(max_x, max_y, max_z, x, y, z);
    output[i] = id;
  }

  return count;
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
