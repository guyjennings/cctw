
// C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ headers
#include <fstream>
#include <iostream>

// CCTW headers
#include <cctwchunkeddatainterface.h>
#include <cctwcrystalcoordinatetransform.h>
#include <cctwtransformer.h>
#include <cctwtransforminterface.h>

// CCTW/Swift header
#include <swift/cctwsimple.h>

using namespace std;

static bool chunkFromBlob(const void *blob,
                          CctwChunkedDataInterface *chunk)
{
  return true;
}

int CctwMaxRequisites()
{
  return CctwTransformer::MAX_REQUISITES;
}

int CctwMaxChunk()
{
  return CctwTransformer::MAX_CHUNK;
}

static void chunkFileName(char *filename,
                          int x, int y, int z, char* d);

static int chunkFileReadBytes(void *output, char *filename);

static const int MAX_FILENAME = 128;

/**
   @return Success: length Failure: -1
 */
int CctwLoadChunk(int max_x, int max_y, int max_z,
                  int x, int y, int z, char* d, void* output)
{
  char filename[MAX_FILENAME];
  chunkFileName(filename, x, y, z, d);
  int result = chunkFileReadBytes(output, filename);
  return result;
}

static void chunkFileName(char *filename,
                          int x, int y, int z, char* d)
{
  sprintf(filename, "%s/chunk-%i-%i-%i.tiff", d, x, y, z);
}

static int fileLength(std::ifstream& stream)
{
  stream.seekg(0, stream.end);
  int length = stream.tellg();
  stream.seekg(0, stream.beg);
  return length;
}

static int chunkFileReadBytes(void *output, char *filename)
{
  // system("pwd");

  ifstream stream(filename, ifstream::binary);
  if (! stream)
  {
    cout << "Could not read from: " << filename << endl;
    return -1;
  }

  int length = fileLength(stream);
  stream.read((char*) output, length);

  if (stream.gcount() != length)
  {
    cout << "error: only " << stream.gcount() <<
        " bytes could be read from " << filename << endl;
    return -1;
  }
  stream.close();

  return length;
}

int CctwXYZtoID(int max_x, int max_y, int max_z,
                int x, int y, int z)
{
  return CctwTransformer::XYZtoID(max_x, max_y, max_z,
                                  x, y, z);
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
