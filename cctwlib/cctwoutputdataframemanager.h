#ifndef CCTWOUTPUTDATAFRAMEMANAGER_H
#define CCTWOUTPUTDATAFRAMEMANAGER_H

#include "cctwdataframemanager.h"
#include "qcepobjectnamer.h"
#include "hdf5.h"
//#include "napi.h"

class CctwDataFrame;

class CctwOutputDataFrameManager : public CctwDataFrameManager
{
  Q_OBJECT
public:
  explicit CctwOutputDataFrameManager(QcepSettingsSaverPtr saver, QObject *parent);

signals:

public slots:
  int  loadChunk(int nx, int ny, int nz);
  void releaseChunk(int chunkId);
  void writeChunk(CctwDataChunk *chunk);

  void beginTransform();
  void endTransform();

private:
  void openOutputFile();
  void closeOutputFile();

private:
  QcepObjectNamer      m_ObjectNamer;
  QcepSettingsSaverPtr m_Saver;
  QMutex               m_WriteLock;
  hid_t                m_FileId;
  hid_t                m_DatasetId;
  hid_t                m_DataspaceId;

public:
  Q_PROPERTY(QString filePath READ get_FilePath WRITE set_FilePath)
  QCEP_STRING_PROPERTY(FilePath)

  Q_PROPERTY(int outputFormat READ get_OutputFormat WRITE set_OutputFormat)
  QCEP_INTEGER_PROPERTY(OutputFormat)
};

#endif // CCTWQTOUTPUTDATAFRAMEMANAGER_H
