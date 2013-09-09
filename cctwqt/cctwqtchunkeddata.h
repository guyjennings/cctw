#ifndef CCTWQTCHUNKEDDATA_H
#define CCTWQTCHUNKEDDATA_H

#include "cctwqtobject.h"
#include <QVector>
#include "cctwqtdatachunk.h"
#include "cctwqtdataframemanager.h"
#include "../cctwchunkeddatainterface.h"

class CctwqtChunkedData : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtChunkedData(CctwChunkedDataInterface *interface, CctwqtDataFrameManager *manager, QObject *parent = 0);
  void allocateChunks();

signals:
  
public slots:
  void clearDependencies();
  void addDependency(CctwIntVector3D f, CctwIntVector3D t);

  CctwqtDataChunk *chunk(CctwIntVector3D idx);

private:
  CctwChunkedDataInterface    *m_Interface;
  CctwqtDataFrameManager      *m_Manager;
  QVector< CctwqtDataChunk* >  m_DataChunks;
};

#endif // CCTWQTCHUNKEDDATA_H
