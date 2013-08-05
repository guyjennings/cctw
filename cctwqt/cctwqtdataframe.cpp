#include "cctwqtdataframe.h"

CctwqtDataFrame::CctwqtDataFrame(int dimx, int dimy, QAtomicInt *refcounter, QObject *parent) :
  CctwqtObject(parent),
  m_Counter(refcounter),
  m_DimX(dimx),
  m_DimY(dimy),
  m_Data(dimx*dimy)
{
  if (m_Counter) {
    m_Counter->fetchAndAddOrdered(1);
  }
}

CctwqtDataFrame::~CctwqtDataFrame()
{
  if (m_Counter) {
    m_Counter->fetchAndAddOrdered(-1);
  }
}
