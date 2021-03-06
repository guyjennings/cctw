#ifndef CCTWIMPORTER_H
#define CCTWIMPORTER_H

#include "cctwobject.h"
#include "qcepimagedata.h"
#include <QSemaphore>
#include "hdf5.h"
#include "cctwintvector3dproperty.h"

class CctwApplication;

class CctwImporter : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwImporter(CctwApplication *application,
                          QString name,
                          QcepObjectWPtr parent);
  virtual ~CctwImporter();

signals:

public slots:
  void importData();
  void loadDarkImage();
  void clearInputFiles();
  void changeDirectory(QString path);
  void appendInputFile(QString path);
  void appendMatchingFiles(QString pattern);
  void checkImportedData();
  void checkImportedDataRigorously();
  void checkImportedDataApproximately();

private slots:
  void importDataFrame(int num, QString path);

private:
  bool createOutputFile();
  void closeOutputFile();
  void writeOutputFrame(int num, QcepImageData<double> *img);

  void initializeDataBuffer();
  void deleteDataBuffer();
  void allocateDataBuffer(hsize_t dimx, hsize_t dimy, hsize_t dimz);
  void readDataFrameToBuffer(int i, int nb, QString path);
  void outputDataFromBuffer(int i);

private:
  CctwApplication *m_Application;
  QMutex           m_OutputMutex;
  QSemaphore       m_BacklogSemaphore;
  QSemaphore       m_CompletionSemaphore;

  hid_t            m_FileId;
  hid_t            m_DatasetId;
  hid_t            m_DataspaceId;

  float           *m_InputBuffer;
  int              m_InputBufferStart;
  int              m_InputBufferCount;
  hsize_t          m_InputBufferSize;
  hsize_t          m_InputBufferStride;

  QcepDoubleImageDataPtr m_DarkImage;

private:
  Q_PROPERTY(int dataFormat READ get_DataFormat WRITE set_DataFormat)
  QCEP_INTEGER_PROPERTY(DataFormat)

  Q_PROPERTY(QString darkImagePath READ get_DarkImagePath WRITE set_DarkImagePath)
  QCEP_STRING_PROPERTY(DarkImagePath)

  Q_PROPERTY(QStringList imagePaths READ get_ImagePaths WRITE set_ImagePaths)
  QCEP_STRING_LIST_PROPERTY(ImagePaths)

  Q_PROPERTY(QString imageDirectory READ get_ImageDirectory WRITE set_ImageDirectory)
  QCEP_STRING_PROPERTY(ImageDirectory)

  Q_PROPERTY(QString imagePattern READ get_ImagePattern WRITE set_ImagePattern)
  QCEP_STRING_PROPERTY(ImagePattern)

  Q_PROPERTY(QString outputPath READ get_OutputPath WRITE set_OutputPath)
  QCEP_STRING_PROPERTY(OutputPath)

  Q_PROPERTY(QString outputDataset READ get_OutputDataset WRITE set_OutputDataset)
  QCEP_STRING_PROPERTY(OutputDataset)

  Q_PROPERTY(CctwIntVector3D chunkSize READ get_ChunkSize WRITE set_ChunkSize)
  CCTW_INTVECTOR3D_PROPERTY(ChunkSize)

  Q_PROPERTY(int compression READ get_Compression WRITE set_Compression)
  QCEP_INTEGER_PROPERTY(Compression)

  Q_PROPERTY(int xDimension READ get_XDimension WRITE set_XDimension STORED false)
  QCEP_INTEGER_PROPERTY(XDimension)

  Q_PROPERTY(int yDimension READ get_YDimension WRITE set_YDimension STORED false)
  QCEP_INTEGER_PROPERTY(YDimension)

  Q_PROPERTY(int zDimension READ get_ZDimension WRITE set_ZDimension STORED false)
  QCEP_INTEGER_PROPERTY(ZDimension)

  Q_PROPERTY(int inputDataBuffering READ get_InputDataBuffering WRITE set_InputDataBuffering)
  QCEP_INTEGER_PROPERTY(InputDataBuffering)

  Q_PROPERTY(bool checkRigorously READ get_CheckRigorously WRITE set_CheckRigorously)
  QCEP_BOOLEAN_PROPERTY(CheckRigorously)

  Q_PROPERTY(bool checkApproximately READ get_CheckApproximately WRITE set_CheckApproximately)
  QCEP_BOOLEAN_PROPERTY(CheckApproximately)
};

#endif // CCTWIMPORTER_H
