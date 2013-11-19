#include "cctwqttransformer.h"
#include <QtConcurrentRun>
#include "cctwqtdatachunk.h"
#include "cctwqtapplication.h"
#include "cctwqtthread.h"

CctwqtTransformer::CctwqtTransformer
  (CctwqtApplication *application,
   CctwInputDataInterface *input,
   CctwOutputDataInterface *output,
   CctwTransformInterface *xform,
   int osx, int osy, int osz,
   QObject *parent) :
  CctwTransformer(input, output, xform, osx, osy, osz, 0, parent),
  m_Application(application),
  m_WallTime(QcepSettingsSaverWPtr(), this, "wallTime", 0, "Wall Time of last command"),
//  m_BlocksAvailable(QcepSettingsSaverWPtr(), this, "blocksAvailable", 0, "Blocks Available"),
//  m_BlocksAllocated(QcepSettingsSaverWPtr(), this, "blocksAllocated", 0, "Blocks Allocated"),
  m_BlocksLimit(m_Application->saver(), this, "blocksLimit", 1000, "Blocks Limit"),
  m_BlocksMax(QcepSettingsSaverWPtr(), this, "blocksMax", 0, "Max Blocks Used")
{
}

void CctwqtTransformer::runTransformChunkNumber(int n)
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

void CctwqtTransformer::transformChunkNumber(int n)
{
  CctwqtCrystalCoordinateTransform transform(m_Application->parameters(), NULL);

  CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);
  CctwIntVector3D lastChunkIndex(-1, -1, -1);
  CctwqtDataChunk *inputChunk = m_InputData->chunk(idx);
  CctwqtDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = m_InputData->chunkStart(idx);
  CctwIntVector3D chSize  = m_InputData->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  if (inputChunk) {
    inputChunk->waitForData();

    QMap<CctwIntVector3D, CctwqtDataChunk*> outputChunks;

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

                CctwqtDataChunk *chunk =
                    new CctwqtDataChunk(m_OutputData, lastChunkIndex, NULL, NULL);

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

    foreach(CctwqtDataChunk *outputChunk, outputChunks) {
      m_OutputData->mergeChunk(outputChunk);

      delete outputChunk;
    }

    inputChunk->finishedWithData();
  }
}

void CctwqtTransformer::transform()
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

  CctwqtDataChunk::resetAllocationLimits(get_BlocksLimit());

  CctwIntVector3D chunks = m_OutputData->chunkCount();

  QVector < int > inputChunks;

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        CctwqtDataChunk *chunk = m_OutputData->chunk(CctwIntVector3D(x,y,z));

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

    QtConcurrent::run(this, &CctwqtTransformer::runTransformChunkNumber, ckidx);
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwqtThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);
  set_BlocksMax(CctwqtDataChunk::maxAllocated());

  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwqtTransformer::checkTransform()
{
  CctwIntVector3D chunks = m_OutputData->chunkCount();

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        CctwqtDataChunk *chunk = m_OutputData->chunk(CctwIntVector3D(x,y,z));

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

QcepIntList CctwqtTransformer::dependencies(int chunkIdx)
{
  CctwqtCrystalCoordinateTransform transform(m_Application->parameters(), NULL);

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

QList<CctwIntVector3D> CctwqtTransformer::dependencies(int cx, int cy, int cz)
{
  QcepIntList deps = dependencies(m_InputData->chunkNumberFromIndex(CctwIntVector3D(cx,cy,cz)));

  QList<CctwIntVector3D> res;

  foreach(int dep, deps) {
    res.append(m_OutputData->chunkIndexFromNumber(dep));
  }

  return res;
}
