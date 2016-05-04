#include "cctwinputdatah5.h"
#include <QFileInfo>

CctwInputDataH5::CctwInputDataH5(QString filePath, QString datasetName, QString name, QcepObjectWPtr parent) :
  CctwObject(name, parent),
  m_InputFilePath(filePath),
  m_DatasetName(datasetName),
  m_FileId(-1),
  m_DatasetId(-1),
  m_DataspaceId(-1)
{
  openInputFile();
}

bool CctwInputDataH5::openInputFile()
{
  QFileInfo f(m_InputFilePath);

  if (!f.exists()) {
    printMessage(tr("File %1 does not exist").arg(m_InputFilePath));
    return false;
  }

  if (!H5Fis_hdf5(qPrintable(m_InputFilePath))) {
    printMessage(tr("File %1 exists but is not an hdf file").arg(m_InputFilePath));
    return false;
  }

  m_FileId = H5Fopen(qPrintable(m_InputFilePath), H5F_ACC_RDONLY, H5P_DEFAULT);

  if (m_FileId < 0) {
    printMessage(tr("File %1 could not be opened").arg(m_InputFilePath));
    return false;
  }

  m_DatasetId = H5Dopen1(m_FileId, qPrintable(m_DatasetName));

  if (m_DatasetId < 0) {
    printMessage(tr("Dataset %1 not found in file %2").arg(m_DatasetName).arg(m_InputFilePath));
    return false;
  }

  m_DataspaceId = H5Dget_space(m_DatasetId);

  if (m_DataspaceId < 0) {
    printMessage(tr("Couldn't get dataspace of dataset %1").arg(m_DatasetName));
    return false;
  }

  int ndims = H5Sget_simple_extent_ndims(m_DataspaceId);

  if (ndims != 3) {
    printMessage(tr("Dataspace is not 3 dimensional (%1)").arg(ndims));
    return false;
  }

  hsize_t dims[3];

  int ndims2 = H5Sget_simple_extent_dims(m_DataspaceId, dims, NULL);

  if (ndims2 != 3) {
    printMessage("Couldnt get dataspace dimensions");
    return false;
  }

  setDimensions(CctwIntVector3D(dims[2], dims[1], dims[0]));

  hid_t pl = H5Dget_create_plist(m_DatasetId);

  if (pl < 0) {
    printMessage("Couldn't get dataset property list");
    return false;
  }

  hsize_t cksz[3];

  int chnks = H5Pget_chunk(pl, 3, cksz);

  if (chnks != 3) {
    printMessage("Chunks are not 3 dimensional");
    return false;
  }

  setChunkSize(CctwIntVector3D(cksz[2], cksz[1], cksz[0]));

  return true;
}

double CctwInputDataH5::readData(int dx, int dy, int dz)
{
  if (m_FileId >= 0) {
    hid_t memspace_id;
    hsize_t offset[3], count[3], stride[3], block[3];
    double data[10];

    count[0] = 1;
    count[1] = 1;
    count[2] = 1;

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    offset[0] = dz;
    offset[1] = dy;
    offset[2] = dx;


    memspace_id = H5Screate_simple(3, count, NULL);

    herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
    herr_t rderr  = H5Dread(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, data);

    if (selerr || rderr) {
      printMessage(tr("Error reading x:%1, y:%2, z:%3, selerr = %4, wrterr = %5")
                   .arg(dx).arg(dy).arg(dz).arg(selerr).arg(rderr));
    }

    return data[0];
  }

  return 0;
}

QVector<double> CctwInputDataH5::readChunk(int ix, int iy, int iz, int nx, int ny, int nz)
{
  QVector<double> data(nx*ny*nz);

  if (m_FileId >= 0) {
    hid_t memspace_id;
    hsize_t offset[3], count[3], stride[3], block[3];

    count[0] = nz;
    count[1] = ny;
    count[2] = nx;

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    offset[0] = iz;
    offset[1] = iy;
    offset[2] = ix;


    memspace_id = H5Screate_simple(3, count, NULL);

    herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
    herr_t rderr  = H5Dread(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, data.data());

    if (selerr || rderr) {
      printMessage(tr("Error reading x:%1, y:%2, z:%3, selerr = %4, wrterr = %5")
                   .arg(ix).arg(iy).arg(iz).arg(selerr).arg(rderr));
    }
  }

  return data;
}

void CctwInputDataH5::setDimensions(CctwIntVector3D dims)
{
  m_Dimensions = dims;
}

void CctwInputDataH5::setChunkSize(CctwIntVector3D cksz)
{
  m_ChunkSize = cksz;
}

CctwIntVector3D CctwInputDataH5::dimensions() const
{
  return m_Dimensions;
}

CctwIntVector3D CctwInputDataH5::chunkSize() const
{
  return m_ChunkSize;
}

//CctwIntVector3D CctwInputDataH5::datasetDimensionsH5(QString filePath, QString datasetName)
//{
//  return CctwIntVector3D();
//}

//CctwIntVector3D CctwInputDataH5::datasetChunkSizeH5(QString filePath, QString datasetName)
//{
//  return CctwIntVector3D();
//}
