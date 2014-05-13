
/*
 * cctwchunkquick.h
 *
 *  Created on: May 12, 2014
 *      Author: wozniak
 */

#ifndef CCTWCHUNKQUICK_H
#define CCTWCHUNKQUICK_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static inline void
assignToChunk(CctwIntVector3D &outputChunkSize,
              int ix, int iy, int iz, double value,
              CctwChunkedData::MergeDataType* chunk)
{
  int y = outputChunkSize.y();
  int z = outputChunkSize.z();
  int index = ix*y*z + iy*z + iz;

  // printf("assignToChunk: %p(%i,%i,%i)[%i] = %0.3f\n", chunk, ix, iy, iz, index, value);
  chunk[index] = (CctwChunkedData::MergeDataType) value;
}

static inline CctwChunkedData::MergeDataType*
makeChunk(CctwIntVector3D &outputChunkSize)
{
  int x = outputChunkSize.x();
  int y = outputChunkSize.y();
  int z = outputChunkSize.z();

  int length = x*y*z*sizeof(CctwChunkedData::MergeDataType);

  // printf("makeChunk: %i\n", length);
  CctwChunkedData::MergeDataType *result =
      (CctwChunkedData::MergeDataType*) malloc(length);
  assert(result != NULL);
  for (int i = 0; i < x; i++)
    for (int j = 0; j < y; j++)
      for (int m = 0; m < z; m++)
        assignToChunk(outputChunkSize, i, j, m, 0, result);

  // printf("makeChunk: done.\n");
  return result;
}

#endif
