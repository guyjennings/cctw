#ifndef CCTWCHUNKINDEX_H
#define CCTWCHUNKINDEX_H

class CctwChunkIndex
{
public:
  CctwChunkIndex(int nx, int ny, int nz);

public:
  int nx() const { return m_Nx; }
  int ny() const { return m_Ny; }
  int nz() const { return m_Nz; }

private:
  int m_Nx, m_Ny, m_Nz;
};

#endif // CCTWCHUNKINDEX_H
