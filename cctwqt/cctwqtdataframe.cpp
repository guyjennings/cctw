#include "cctwqtdataframe.h"

CctwqtDataFrame::CctwqtDataFrame(int dimx, int dimy, QObject *parent) :
  CctwqtObject(parent),
  m_Counter(0),
  m_DimX(dimx),
  m_DimY(dimy),
  m_Data(dimx*dimy)
{
}

CctwqtDataFrame::~CctwqtDataFrame()
{
}

int CctwqtDataFrame::reference()
{
  return m_Counter.fetchAndAddOrdered(1);
}

int CctwqtDataFrame::dereference()
{
  return m_Counter.fetchAndAddOrdered(-1);
}
