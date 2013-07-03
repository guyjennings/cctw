#ifndef CCTWOUTPUTCHUNKINDEX_H
#define CCTWOUTPUTCHUNKINDEX_H

class CctwOutputChunkIndex
{
public:
  CctwOutputChunkIndex(int nx, int ny, int nz);

public:
  static CctwOutputChunkIndex createNew(int argc, char *argv[]);

  int nx() const { return m_Nx; }
  int ny() const { return m_Ny; }
  int nz() const { return m_Nz; }

private:
  int m_Nx, m_Ny, m_Nz;
};

#endif // CCTWOUTPUTCHUNKINDEX_H
