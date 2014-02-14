#ifndef CCTWINPUTDATAH5_H
#define CCTWINPUTDATAH5_H

#include "cctwobject.h"
#include "hdf5.h"
#include "cctwvector3d.h"

class CctwInputDataH5 : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwInputDataH5(QString filePath, QString datasetName, CctwObject *parent);

signals:

public slots:
  virtual double readData(int dx, int dy, int dz);
  QVector<double> readChunk(int ix, int iy, int iz,
                            int nx, int ny, int nz);

public:
  CctwIntVector3D dimensions() const;
  CctwIntVector3D chunkSize() const;

private:
  bool openInputFile();
//  CctwIntVector3D datasetDimensionsH5(QString filePath, QString datasetName);
//  CctwIntVector3D datasetChunkSizeH5(QString filePath, QString datasetName);

  void setDimensions(CctwIntVector3D dims);
  void setChunkSize(CctwIntVector3D cksz);

private:
  CctwIntVector3D  m_Dimensions;
  CctwIntVector3D  m_ChunkSize;
  QString          m_InputFilePath;
  QString          m_DatasetName;
  hid_t            m_FileId;
  hid_t            m_DatasetId;
  hid_t            m_DataspaceId;
};

#endif // CCTWINPUTDATAH5_H
