#ifndef CCTWQTDATACHUNK_H
#define CCTWQTDATACHUNK_H

#include "cctwqtobject.h"
#include <QVector>
#include <QSharedPointer>
#include "cctwqtdataframe.h"
#include "cctwchunkeddatainterface.h"
#include "cctwqtdataframemanager.h"

class CctwqtDataChunk : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataChunk(CctwChunkedDataInterface *data, CctwqtDataFrameManager *manager, QObject *parent);
  
signals:
  
public slots:
  
private:
  CctwChunkedDataInterface                    *m_Data;
  CctwqtDataFrameManager                      *m_Manager;
  QVector< QSharedPointer <CctwqtDataFrame> >  m_DataFrames;
};

#endif // CCTWQTDATACHUNK_H
