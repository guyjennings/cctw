#ifndef CCTWOUTPUTCHUNKINDEX_H
#define CCTWOUTPUTCHUNKINDEX_H

#include "cctwchunkindex.h"

class CctwOutputChunkIndex : public CctwChunkIndex
{
public:
  CctwOutputChunkIndex(int nx, int ny, int nz);

public:
  static CctwOutputChunkIndex createNew(int argc, char *argv[]);
};

#endif // CCTWOUTPUTCHUNKINDEX_H
