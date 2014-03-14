#include "cctwtransformer.h"
#include "cctwcommandline.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cctwmatrix3x3.h"
#include "cctwapplication.h"
#include <QtConcurrentRun>
#include "cctwthread.h"
#include "cctwdatachunk.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QUrlQuery>
#endif

CctwTransformer::CctwTransformer(CctwApplication        *application,
                                 CctwInputData          *input,
                                 CctwOutputData         *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz, int nTests, QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Application(application),
  m_InputData(input),
  m_OutputData(output),
  m_Transform(xform),
  m_OversampleX(osx),
  m_OversampleY(osy),
  m_OversampleZ(osz),
  m_Tests(nTests),
  m_ChunkCount(input->chunkCount()),
  m_ChunksTotal(m_ChunkCount.x()*m_ChunkCount.y()*m_ChunkCount.z()),
  m_ChunksUsed(NULL),
  m_WallTime(QcepSettingsSaverWPtr(), this, "wallTime", 0, "Wall Time of last command"),
  //  m_BlocksAvailable(QcepSettingsSaverWPtr(), this, "blocksAvailable", 0, "Blocks Available"),
  //  m_BlocksAllocated(QcepSettingsSaverWPtr(), this, "blocksAllocated", 0, "Blocks Allocated"),
  m_BlocksLimit(m_Application->saver(), this, "blocksLimit", 1000, "Blocks Limit"),
  m_BlocksMax(QcepSettingsSaverWPtr(), this, "blocksMax", 0, "Max Blocks Used")
{
  m_ChunksUsed = new int[m_ChunksTotal];
}

CctwTransformer::~CctwTransformer()
{
  delete [] m_ChunksUsed;
}

void CctwTransformer::performTests()
{
  printf("Performing tests\n");

#ifndef WIN32
  for (int i=0; i<m_Tests; i++) {
    {
      double rot1 = 2.0*M_PI*(double)random()/(double)RAND_MAX;
      double rot2 = 2.0*M_PI*(double)random()/(double)RAND_MAX;
      double rot3 = 2.0*M_PI*(double)random()/(double)RAND_MAX;

      bool invertible;

      CctwDoubleMatrix3x3 m1 = CctwDoubleMatrix3x3::rotationMatrix(rot1, rot2, rot3);
      CctwDoubleMatrix3x3 m2 = m1.inverted(&invertible);
      CctwDoubleMatrix3x3 m3 = m1*m2;

      double det = m3.determinant();

      double sum1 = fabs(m3(0,0)) + fabs(m3(1,1)) + fabs(m3(2,2));
      double sum2 = fabs(m3(0,1)) + fabs(m3(0,2)) + fabs(m3(1,2));
      double sum3 = fabs(m3(1,0)) + fabs(m3(2,0)) + fabs(m3(2,1));

      printf("Iter %d.0: Det %g, Diag %g, Upper %g, Lower %g\n", i, det, sum1, sum2 ,sum3);

      double x = (double)random();
      double y = (double)random();
      double z = (double)random();

      CctwDoubleVector3D v1(x,y,z);
      CctwDoubleVector3D v2 = m1*v1;
      CctwDoubleVector3D v3 = m2*v2;

      double sum4 = (v1 - v3).length();

      printf("Iter %d.1: Len %g\n", i, sum4);
    }

    {
      double a = (double)random();
      double b = (double)random();
      double c = (double)random();
      double d = (double)random();
      double e = (double)random();
      double f = (double)random();
      double g = (double)random();
      double h = (double)random();
      double k = (double)random();

      bool invertible;

      CctwDoubleMatrix3x3 m1(a,b,c,d,e,f,g,h,k);
      CctwDoubleMatrix3x3 m2 = m1.inverted(&invertible);
      CctwDoubleMatrix3x3 m3 = m1*m2;

      double det = m3.determinant();

      double sum1 = fabs(m3(0,0)) + fabs(m3(1,1)) + fabs(m3(2,2));
      double sum2 = fabs(m3(0,1)) + fabs(m3(0,2)) + fabs(m3(1,2));
      double sum3 = fabs(m3(1,0)) + fabs(m3(2,0)) + fabs(m3(2,1));

      printf("Iter %d.2: Det %g, Diag %g, Upper %g, Lower %g\n", i, det, sum1, sum2 ,sum3);
    }
  }
#endif
}

void CctwTransformer::runTransformChunkNumber(int n)
{
  if (m_Application && !m_Application->get_Halting()) {
    transformChunkNumber(n);

    m_MergeCounter.fetchAndAddOrdered(-1);
  } else {
    m_MergeCounter.fetchAndStoreOrdered(0);
  }

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwTransformer::transformChunkNumber(int n)
{
  CctwCrystalCoordinateTransform transform(m_Application->parameters(), tr("transform-%1").arg(n), NULL);

  CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);
  CctwIntVector3D lastChunkIndex(-1, -1, -1);
  CctwDataChunk *inputChunk = m_InputData->chunk(n);
  CctwDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = m_InputData->chunkStart(idx);
  CctwIntVector3D chSize  = m_InputData->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  if (inputChunk) {
    inputChunk->waitForData();

    QMap<CctwIntVector3D, CctwDataChunk*> outputChunks;

    inputChunk->readData();
    inputChunk->readWeights();

    for (int z=0; z<chSize.z(); z++) {
      for (int y=0; y<chSize.y(); y++) {
        for (int x=0; x<chSize.x(); x++) {
          CctwIntVector3D iprelat(x,y,z);
          CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x,y,z);
          CctwDoubleVector3D xfmcoord = transform.forward(coords);
          CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

          if (m_OutputData->containsPixel(pixels)) {
            CctwIntVector3D opchunk = m_OutputData->chunkIndex(pixels);
            CctwIntVector3D oprelat = pixels - m_OutputData->chunkStart(opchunk);

            if (opchunk != lastChunkIndex) {

              lastChunkIndex = opchunk;

              if (!outputChunks.contains(lastChunkIndex)) {
//                printMessage(tr("Input Chunk [%1,%2,%3] -> Output Chunk [%4,%5,%6]")
//                             .arg(idx.x()).arg(idx.y()).arg(idx.z())
//                             .arg(opchunk.x()).arg(opchunk.y()).arg(opchunk.z()));

                CctwDataChunk *chunk =
                    new CctwDataChunk(m_OutputData, lastChunkIndex,
                                      tr("chunk-%1").arg(lastChunkIndex.toString()), NULL);

                if (chunk) {
                  chunk->allocateData();
                  chunk->allocateWeights();
                }

                outputChunks[lastChunkIndex] = chunk;
              }

              lastChunk = outputChunks[lastChunkIndex];
            }

            if (lastChunk) {
              double oval = lastChunk->data(oprelat);
              double owgt = lastChunk->weight(oprelat);
              double ival = inputChunk->data(iprelat);
              double iwgt = inputChunk->weight(iprelat);

              if (iwgt != 0) {
                lastChunk->setData(oprelat, oval+ival);
                lastChunk->setWeight(oprelat, owgt+iwgt);
              }
            }
          }
        }
      }
    }

    inputChunk->deallocateData();
    inputChunk->deallocateWeights();

    //      printMessage(tr("Need to merge %1 output chunks from input chunk [%2,%3,%4]")
    //                   .arg(outputChunks.count())
    //                   .arg(idx.x()).arg(idx.y()).arg(idx.z()));

    if (inputChunk->dependencyCount() != outputChunks.count()) {
      printMessage(tr("Discrepancy between numbers of merged chunks : dependencyCount() = %1, chunks.count() = %2")
                   .arg(inputChunk->dependencyCount()).arg(outputChunks.count()));
    }

    foreach(CctwDataChunk *outputChunk, outputChunks) {
      m_OutputData->mergeChunk(outputChunk);

      delete outputChunk;
    }

    inputChunk->finishedWithData();
  }
}

void CctwTransformer::transform()
{
  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
  }

  QTime startAt;

  startAt.start();

  printMessage("Starting Transform");

  m_OutputData->beginTransform();

  m_MergeCounter.fetchAndStoreOrdered(0);

  m_InputData->clearMergeCounters();
  m_OutputData->clearMergeCounters();

  CctwDataChunk::resetAllocationLimits(get_BlocksLimit());

  CctwIntVector3D chunks = m_OutputData->chunkCount();

  QVector < int > inputChunks;

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        CctwDataChunk *chunk = m_OutputData->chunk(CctwIntVector3D(x,y,z));

        if (chunk) {
          int n = chunk->dependencyCount();

          for (int i=0; i<n; i++) {
            int ckidx = m_InputData->chunkNumberFromIndex(chunk->dependency(i));

            if (!inputChunks.contains(ckidx)) {
              inputChunks.append(ckidx);
            }
          }
        }
      }
    }
  }

  printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));

  qSort(inputChunks.begin(), inputChunks.end());

  if (m_Application) {
    m_Application->set_ProgressLimit(inputChunks.count());
  }

  foreach(int ckidx, inputChunks) {
    m_MergeCounter.fetchAndAddOrdered(1);

    if (m_Application) {
      m_Application->addWorkOutstanding(1);
    }

    QtConcurrent::run(this, &CctwTransformer::runTransformChunkNumber, ckidx);
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);
  set_BlocksMax(CctwDataChunk::maxAllocated());

  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwTransformer::checkTransform()
{
  CctwIntVector3D chunks = m_OutputData->chunkCount();

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        CctwDataChunk *chunk = m_OutputData->chunk(CctwIntVector3D(x,y,z));

        if (chunk) {
          if (chunk->dependencyCount() != chunk->mergeCount()) {
            printMessage(tr("Anomaly in chunk [%1,%2,%3] : deps %4, merge %5")
                         .arg(x).arg(y).arg(z)
                         .arg(chunk->dependencyCount())
                         .arg(chunk->mergeCount()));
          }
        }
      }
    }
  }
}

QcepIntList CctwTransformer::dependencies(int chunkIdx)
{
  CctwCrystalCoordinateTransform transform(m_Application->parameters(), tr("transform-%1").arg(chunkIdx), NULL);

  CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(chunkIdx);
  CctwIntVector3D lastChunkIndex(-1, -1, -1);

  CctwIntVector3D chStart = m_InputData->chunkStart(idx);
  CctwIntVector3D chSize  = m_InputData->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  QList<CctwIntVector3D> outputChunks;
  QcepIntList            result;

  if (m_InputData->containsChunk(idx)) {
    for (int z=0; z<chSize.z(); z++) {
      for (int y=0; y<chSize.y(); y++) {
        for (int x=0; x<chSize.x(); x++) {
          CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x,y,z);
          CctwDoubleVector3D xfmcoord = transform.forward(coords);
          CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

          if (m_OutputData->containsPixel(pixels)) {
            CctwIntVector3D opchunk = m_OutputData->chunkIndex(pixels);

            if (opchunk != lastChunkIndex) {
              lastChunkIndex = opchunk;

              if (!outputChunks.contains(lastChunkIndex)) {
                outputChunks.append(opchunk);
              }
            }
          }
        }
      }
    }
  }

  foreach(CctwIntVector3D chk, outputChunks) {
    result.append(m_OutputData->chunkNumberFromIndex(chk));
  }

  qSort(result);

  return result;
}

QList<CctwIntVector3D> CctwTransformer::dependencies(int cx, int cy, int cz)
{
  QcepIntList deps = dependencies(m_InputData->chunkNumberFromIndex(CctwIntVector3D(cx,cy,cz)));

  QList<CctwIntVector3D> res;

  foreach(int dep, deps) {
    res.append(m_OutputData->chunkIndexFromNumber(dep));
  }

  return res;
}

//CctwIntermediateDataBlob*        CctwTransformer::mergeBlobs(CctwIntermediateDataBlob *blob1, CctwIntermediateDataBlob *blob2)
//{
//  int chunkId = blob1->blobID();

//  CctwIntermediateDataBlob* res = CctwIntermediateDataBlob::newIntermediateDataBlob(chunkId, m_OutputData->chunkSize());

//  double *data = res->data();
//  double *weight = res->weight();

//  double *data1 = blob1->data();
//  double *weight1 = blob1->weight();

//  double *data2 = blob2->data();
//  double *weight2 = blob2->weight();

//  if (data && weight && data1 && weight1 && data2 && weight2) {
//    int len = res->dataLength();

//    if ((res->weightLength() == len) &&
//        (blob1->dataLength() == len) &&
//        (blob1->weightLength() == len) &&
//        (blob2->dataLength() == len) &&
//        (blob2->weightLength() == len))
//    {
//      for(int i=0; i<len; i++) {
//        data[i] = 0;
//        weight[i] = 0;

//        if (weight1[i]) {
//          data[i] += data1[i];
//          weight[i] += weight1[i];
//        }

//        if (weight2[i]) {
//          data[i] += data2[i];
//          weight[i] += weight2[i];
//        }
//      }

//      return res;
//    } else {
//      printMessage("Bad Merge Data Lengths");
//    }
//  } else {
//    printMessage("Bad Merge Data");
//  }

//  return res;
//}

//CctwOutputDataBlob*              CctwTransformer::normalizeBlob(CctwIntermediateDataBlob *blob)
//{
//  int chunkId = blob->blobID();

//  CctwOutputDataBlob* res = CctwOutputDataBlob::newOutputDataBlob(chunkId, m_OutputData->chunkSize());

//  double *out = res->data();
//  double *data = blob->data();
//  double *weight = blob->weight();

//  if (out && data && weight) {
//    int len = res->dataLength();

//    if ((blob->dataLength() == len) && (blob->weightLength() == len)) {
//      for (int i=0; i<len; i++) {
//        if (weight[i]) {
//          out[i] = data[i]/weight[i];
//        } else {
//          out[i] = 0;
//        }
//      }
//    }
//  }

//  return res;
//}

//void CctwTransformer::readHDF5InputBlob(int blobIdx, QUrl location, CctwInputDataBlob *blob)
//{
//  QString filePath = location.path();
//  QString dataset  = location.fragment();

//  printf("CctwTransformer::readHDF5InputBlob\n");

//  if (filePath.count() > 0) {
//    if (dataset.count() == 0) {
//      dataset = "data";
//    }

//    hid_t fileId = H5Fopen(qPrintable(filePath), H5F_ACC_RDONLY, H5P_DEFAULT);

//    if (fileId < 0) {
//      printMessage(tr("File open of %1 failed").arg(filePath));
//    } else {
//      hid_t datasetId = H5Dopen(fileId, qPrintable(dataset), H5P_DEFAULT);

//      if (datasetId < 0) {
//        printMessage(tr("Dataset %1 not opened").arg(dataset));
//      } else {
//        hid_t dataspaceId = H5Dget_space(datasetId);

//        if (dataspaceId < 0) {
//          printMessage(tr("Couldn't get data space"));
//        } else {
//          int ndims = H5Sget_simple_extent_ndims(dataspaceId);

//          if (ndims != 3) {
//            printMessage(tr("Data dimensionality != 3 (%1)").arg(ndims));
//          } else {
//            hsize_t dims[3], maxdims[3];
//            int n = H5Sget_simple_extent_dims(dataspaceId, dims, maxdims);

//            if (n != 3) {
//              printMessage("Problems getting dataset dimensions");
//            } else {
//              printMessage(tr("Dimensions [%1,%2,%3]").arg(dims[0]).arg(dims[1]).arg(dims[2]));

//              printf("About to read data\n");

//              hid_t memspace_id;
//              hsize_t offset[3], count[3], stride[3], block[3];

//              CctwIntVector3D ckoffset = m_InputData->chunkStart(m_InputData->chunkIndexFromNumber(blobIdx));
//              CctwIntVector3D cksize   = m_InputData->chunkSize();

//              count[0] = cksize.z();
//              count[1] = cksize.y();
//              count[2] = cksize.x();

//              stride[0] = 1;
//              stride[1] = 1;
//              stride[2] = 1;

//              block[0] = 1;
//              block[1] = 1;
//              block[2] = 1;

//              offset[0] = ckoffset.z();
//              offset[1] = ckoffset.y();
//              offset[2] = ckoffset.x();

//              memspace_id = H5Screate_simple(3, count, NULL);

//              herr_t selerr = H5Sselect_hyperslab(dataspaceId, H5S_SELECT_SET, offset, stride, count, block);
//              herr_t rderr  = H5Dread(datasetId, H5T_NATIVE_DOUBLE, memspace_id, dataspaceId, H5P_DEFAULT, blob->data());

//              if (selerr || rderr) {
//                printMessage(tr("Error reading x:%1, y:%2, z:%3, selerr = %4, wrterr = %5")
//                             .arg(ckoffset.x()).arg(ckoffset.y()).arg(ckoffset.z()).arg(selerr).arg(rderr));
//                printf("Failed to read data\n");
//              }
//            }
//          }
//        }
//      }

//      H5Fclose(fileId);
//    }
//  }
//}
