#include "cctwqtdatachunk.h"

CctwqtDataChunk::CctwqtDataChunk(CctwChunkedDataInterface *data, CctwqtDataFrameManager *manager) :
  QObject(),
  m_Data(data),
  m_Manager(manager)
{
  int n = m_Data->chunkCount().z();

  m_DataFrames.resize(n);
}
