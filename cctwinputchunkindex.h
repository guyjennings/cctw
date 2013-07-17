#ifndef CCTWINPUTCHUNKINDEX_H
#define CCTWINPUTCHUNKINDEX_H

#include "cctwchunkindex.h"

class CctwInputChunkIndex : public CctwChunkIndex
{
public:
  CctwInputChunkIndex(int nx, int ny, int nz);

public:
  static CctwInputChunkIndex createNew(int argc, char *argv[]);
};

#endif // CCTWINPUTCHUNKINDEX_H
