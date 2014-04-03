#include "cctwtransformer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cctwmatrix3x3.h"
#include "cctwapplication.h"
#include <QtConcurrentRun>
#include "cctwthread.h"
#include "cctwdatachunk.h"
#include "qcepmutexlocker.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QUrlQuery>
#endif

CctwTransformer::CctwTransformer(CctwApplication        *application,
                                 CctwChunkedData *input,
                                 CctwChunkedData *output,
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
  m_WallTime(QcepSettingsSaverWPtr(), this, "wallTime", 0, "Wall Time of last command"),
  m_BlocksLimit(m_Application->saver(), this, "blocksLimit", 1000, "Blocks Limit"),
  m_TransformOptions(m_Application->saver(), this, "transformOptions", 0, "Transform Options")
{
  qRegisterMetaType< CctwDependencies >("CctwDependencies");
  qRegisterMetaTypeStreamOperators< CctwDependencies >("CctwDependencies");
}

CctwTransformer::~CctwTransformer()
{
}

void CctwTransformer::writeSettings(QSettings *set, QString section)
{
  CctwObject::writeSettings(set, section);

  set->beginGroup(section+"/dependencies");
//  set->setValue("dependencies", m_Dependencies);
  set->endGroup();
}

void CctwTransformer::readSettings(QSettings *set, QString section)
{
  CctwObject::readSettings(set, section);

  set->beginGroup(section+"/dependencies");
  set->endGroup();
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

  int lastChunkIndex = -1;

  CctwDataChunk *inputChunk = m_InputData->readChunk(n);
  CctwDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = inputChunk->chunkStart();
  CctwIntVector3D chSize  = inputChunk->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  if (inputChunk) {
    QMap<int, CctwDataChunk*> outputChunks;

    for (int z=0; z<chSize.z(); z++) {
      for (int y=0; y<chSize.y(); y++) {
        for (int x=0; x<chSize.x(); x++) {
          CctwIntVector3D iprelat(x,y,z);
          CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x,y,z);
          CctwDoubleVector3D xfmcoord = transform.forward(coords);
          CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

          if (m_OutputData->containsPixel(pixels)) {
            int opchunk = m_OutputData->chunkContaining(pixels);
            CctwIntVector3D oprelat = pixels - m_OutputData->chunkStart(opchunk);

            if (opchunk != lastChunkIndex) {

              lastChunkIndex = opchunk;

              if (!outputChunks.contains(lastChunkIndex)) {
//                printMessage(tr("Input Chunk [%1,%2,%3] -> Output Chunk [%4,%5,%6]")
//                             .arg(idx.x()).arg(idx.y()).arg(idx.z())
//                             .arg(opchunk.x()).arg(opchunk.y()).arg(opchunk.z()));

                CctwDataChunk *chunk =
                    new CctwDataChunk(m_OutputData, lastChunkIndex,
                                      tr("chunk-%1").arg(lastChunkIndex), NULL);

                if (chunk) {
                  chunk->allocateData();
                  chunk->allocateWeights();
                }

                outputChunks[lastChunkIndex] = chunk;
              }

              lastChunk = outputChunks[lastChunkIndex];
            }

            if (lastChunk) {
              int ox = oprelat.x(), oy = oprelat.y(), oz = oprelat.z();
              int ix = iprelat.x(), iy = iprelat.y(), iz = iprelat.z();

              double oval = lastChunk->data(ox, oy, oz);
              double owgt = lastChunk->weight(ox, oy, oz);
              double ival = inputChunk->data(ix, iy, iz);
              double iwgt = inputChunk->weight(ix, iy, iz);

              if (iwgt != 0) {
                lastChunk->setData(ox, oy, oz, oval+ival);
                lastChunk->setWeight(ox, oy, oz, owgt+iwgt);
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

    m_InputData->releaseChunk(n);
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

  m_InputData  -> beginTransform(true,  get_TransformOptions());
  m_OutputData -> beginTransform(false, get_TransformOptions());

  m_MergeCounter.fetchAndStoreOrdered(0);

  m_InputData  -> clearMergeCounters();
  m_OutputData -> clearMergeCounters();

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
            int ckidx = chunk->dependency(i);

            if (!inputChunks.contains(ckidx)) {
              inputChunks.append(ckidx);
            }
          }
        }
      }
    }
  }

  printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));

  if ((get_TransformOptions() & 2)) {
    printMessage("Sorting input chunk list into input order");
    qSort(inputChunks.begin(), inputChunks.end());
  }

  if (m_Application) {
    m_Application->set_ProgressLimit(inputChunks.count());
  }

  foreach(int ckidx, inputChunks) {
    m_MergeCounter.fetchAndAddOrdered(1);

    if (m_Application) {
      m_Application->addWorkOutstanding(1);
    }

    if ((get_TransformOptions() & 1) == 0) {
      QtConcurrent::run(this, &CctwTransformer::runTransformChunkNumber, ckidx);
    } else {
      runTransformChunkNumber(ckidx);
    }
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);

  m_InputData  -> endTransform();
  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwTransformer::dummyTransformChunkNumber(int chk)
{
  CctwDataChunk *chunk = m_InputData->chunk(chk);

  if (chunk) {
    m_InputData->incChunksRead(1);

    int ndeps = chunk->dependencyCount();

    for (int i=0; i<ndeps; i++) {
      int ochk = chunk->dependency(i);

      CctwDataChunk *opchunk = m_OutputData->chunk(ochk);

      if (opchunk) {
        opchunk->incMergeCounters();

        if (opchunk->mergeCount() == opchunk->dependencyCount()) {
          m_OutputData->incChunksWritten(1);
          m_OutputData->incChunksHeld(-1);
        } else if (opchunk->mergeCount() == 1) {
          m_OutputData->incChunksHeld(1);
        }
      }
    }
  }
}

void CctwTransformer::runDummyTransformChunkNumber(int n)
{
  if (m_Application && !m_Application->get_Halting()) {
    dummyTransformChunkNumber(n);

    m_MergeCounter.fetchAndAddOrdered(-1);
  } else {
    m_MergeCounter.fetchAndStoreOrdered(0);
  }

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwTransformer::dummyTransform1()
{
  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
  }

  QTime startAt;

  startAt.start();

  printMessage("Starting Dummy Transform type 1");

  m_InputData  -> beginTransform(true,  get_TransformOptions());
  m_OutputData -> beginTransform(false, get_TransformOptions());

  m_MergeCounter.fetchAndStoreOrdered(0);

  m_InputData  -> clearMergeCounters();
  m_OutputData -> clearMergeCounters();

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
            int ckidx = chunk->dependency(i);

            if (!inputChunks.contains(ckidx)) {
              inputChunks.append(ckidx);
            }
          }
        }
      }
    }
  }

  printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));

  if ((get_TransformOptions() & 2)) {
    printMessage("Sorting input chunk list into input order");
    qSort(inputChunks.begin(), inputChunks.end());
  }

  if (m_Application) {
    m_Application->set_ProgressLimit(inputChunks.count());
  }

  foreach(int ckidx, inputChunks) {
    m_MergeCounter.fetchAndAddOrdered(1);

    if (m_Application) {
      m_Application->addWorkOutstanding(1);
    }

//    QtConcurrent::run(this, &CctwTransformer::runDummyTransformChunkNumber, ckidx);

    runDummyTransformChunkNumber(ckidx);
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);
//  set_BlocksMax(CctwDataChunk::maxAllocated());

  m_InputData  -> endTransform();
  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwTransformer::dummyTransform2()
{
  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
  }

  QTime startAt;

  startAt.start();

  printMessage("Starting Dummy Transform type 2");

  m_InputData  -> beginTransform(true,  get_TransformOptions());
  m_OutputData -> beginTransform(false, get_TransformOptions());

  m_MergeCounter.fetchAndStoreOrdered(0);

  m_InputData  -> clearMergeCounters();
  m_OutputData -> clearMergeCounters();

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
            int ckidx = chunk->dependency(i);

            if (!inputChunks.contains(ckidx)) {
              inputChunks.append(ckidx);
            }
          }
        }
      }
    }
  }

  printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));

//  qSort(inputChunks.begin(), inputChunks.end());

  if (m_Application) {
    m_Application->set_ProgressLimit(inputChunks.count());
  }

  foreach(int ckidx, inputChunks) {
    m_MergeCounter.fetchAndAddOrdered(1);

    if (m_Application) {
      m_Application->addWorkOutstanding(1);
    }

    //    QtConcurrent::run(this, &CctwTransformer::runDummyTransformChunkNumber, ckidx);

    runDummyTransformChunkNumber(ckidx);
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);
//  set_BlocksMax(CctwDataChunk::maxAllocated());

  m_InputData  -> endTransform();
  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwTransformer::dummyTransform3()
{
  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
  }

  QTime startAt;

  startAt.start();

  printMessage("Starting Dummy Transform type 3");

  m_InputData  -> beginTransform(true,  get_TransformOptions());
  m_OutputData -> beginTransform(false, get_TransformOptions());

  m_MergeCounter.fetchAndStoreOrdered(0);

  m_InputData  -> clearMergeCounters();
  m_OutputData -> clearMergeCounters();

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
            int ckidx = chunk->dependency(i);

            if (!inputChunks.contains(ckidx)) {
              inputChunks.append(ckidx);
            }
          }
        }
      }
    }
  }

  printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));

//  qSort(inputChunks.begin(), inputChunks.end());

  if (m_Application) {
    m_Application->set_ProgressLimit(inputChunks.count());
  }

  foreach(int ckidx, inputChunks) {
    m_MergeCounter.fetchAndAddOrdered(1);

    if (m_Application) {
      m_Application->addWorkOutstanding(1);
    }

    //    QtConcurrent::run(this, &CctwTransformer::runDummyTransformChunkNumber, ckidx);

    runDummyTransformChunkNumber(ckidx);
  }

  while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    m_Application->processEvents();
  }

  set_WallTime(startAt.elapsed()/1000.0);
//  set_BlocksMax(CctwDataChunk::maxAllocated());

  m_InputData  -> endTransform();
  m_OutputData -> endTransform();

  printMessage(tr("Transform complete after %1 sec").arg(get_WallTime()));
}

void CctwTransformer::checkTransform()
{
  int chunks = m_OutputData->chunkCount().volume();

  for (int i = 0; i<chunks; i++) {
    CctwDataChunk *chunk = m_OutputData->chunk(i);

    if (chunk) {
      if (chunk->dependencyCount() != chunk->mergeCount()) {
        printMessage(tr("Anomaly in chunk [%1] : deps %2, merge %3")
                     .arg(i)
                     .arg(chunk->dependencyCount())
                     .arg(chunk->mergeCount()));
      }

      if (chunk->dataPointer()) {
        printMessage(tr("Chunk [%1] still has allocated data").arg(i));
      }

      if (chunk->weightsPointer()) {
        printMessage(tr("Chunk [%1] still has allocated weights").arg(i));
      }
    }
  }
}

QcepIntList CctwTransformer::dependencies(int n)
{
  CctwCrystalCoordinateTransform transform(m_Application->parameters(), tr("transform-%1").arg(n), NULL);

  CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);

  int lastChunkIndex = -1;

  CctwDataChunk   *chunk = m_InputData->chunk(n);

  QList<int>      outputChunks;
  QcepIntList     result;

  if (chunk) {
    CctwIntVector3D chStart = chunk->chunkStart();
    CctwIntVector3D chSize  = chunk->chunkSize();
    CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

    if (m_InputData->containsChunk(idx.x(), idx.y(), idx.z())) {
      for (int z=0; z<chSize.z(); z++) {
        for (int y=0; y<chSize.y(); y++) {
          for (int x=0; x<chSize.x(); x++) {
            CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x,y,z);
            CctwDoubleVector3D xfmcoord = transform.forward(coords);
            CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

            if (m_OutputData->containsPixel(pixels)) {
              int opchunk = m_OutputData->chunkContaining(pixels);

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
  }

  foreach(int chk, outputChunks) {
    result.append(chk);
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

void CctwTransformer::clearDependencies()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_DependencyMutex);

  m_Dependencies.clear();
}

void CctwTransformer::addDependency(int f, int t)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_DependencyMutex);

  CctwDependency dep(f,t);

  if (!m_Dependencies.contains(dep)) {
    m_Dependencies.append(dep);
  }
}

void CctwTransformer::completedDependencies()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_DependencyMutex);

  m_InputData->clearDependencies();
  m_OutputData->clearDependencies();

  foreach (CctwDependency p, m_Dependencies) {
    m_InputData->addDependency(p.first, p.second);
    m_OutputData->addDependency(p.second, p.first);
  }
}

int CctwTransformer::countDependencies()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_DependencyMutex);

  return m_Dependencies.count();
}
