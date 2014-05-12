#include "cctwtransformer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cctwmatrix3x3.h"
#include "cctwapplication.h"
#include "cctwchunkquick.h"
#include <QtConcurrentRun>
#include "cctwthread.h"
#include "cctwdatachunk.h"
#include "qcepmutexlocker.h"
#include "qcepimagedataformattiff.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QUrlQuery>
#endif

CctwTransformer::CctwTransformer(CctwApplication        *application,
                                 CctwChunkedData *input,
                                 CctwChunkedData *output,
                                 CctwTransformInterface *xform,
                                 /*int osx, int osy, int osz, */QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Application(application),
  m_InputData(input),
  m_OutputData(output),
  m_Transform(xform),
//  m_OversampleX(osx),
//  m_OversampleY(osy),
//  m_OversampleZ(osz),
  m_ImageX(NULL),
  m_ImageY(NULL),
  m_ImageZ(NULL),
  m_WallTime(QcepSettingsSaverWPtr(), this, "wallTime", 0, "Wall Time of last command"),
  m_BlocksLimit(m_Application->saver(), this, "blocksLimit", 1000, "Blocks Limit"),
  m_TransformOptions(m_Application->saver(), this, "transformOptions", 0, "Transform Options"),
  m_OversampleX(m_Application->saver(), this, "oversampleX", 1, "Oversampling along X"),
  m_OversampleY(m_Application->saver(), this, "oversampleY", 1, "Oversampling along Y"),
  m_OversampleZ(m_Application->saver(), this, "oversampleZ", 1, "Oversampling along Z"),
  m_ProjectX(m_Application->saver(), this, "projectX", true, "Project along X"),
  m_ProjectY(m_Application->saver(), this, "projectY", true, "Project along Y"),
  m_ProjectZ(m_Application->saver(), this, "projectZ", true, "Project along Z"),
  m_ProjectDestination(m_Application->saver(), this, "projectDestination", "", "Output path for projected images")
{
}

CctwTransformer::~CctwTransformer()
{
}

void CctwTransformer::writeSettings(QSettings *set, QString section)
{
  CctwObject::writeSettings(set, section);
}

void CctwTransformer::readSettings(QSettings *set, QString section)
{
  CctwObject::readSettings(set, section);
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


void CctwTransformer::saveDependencies(QString path)
{
  QFile f(path);

  if (f.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream s(&f);

    int nchunk = m_InputData->chunkCount().volume();

    for (int i=0; i<nchunk; i++) {
      CctwDataChunk *chunk = m_InputData->chunk(i);

      chunk->sortDependencies();

      int n = chunk->dependencyCount();

      for (int j=0; j<n; j++) {
        int o = chunk->dependency(j);

        s << i << "\t" << o << endl;
      }
    }
  }
}

void CctwTransformer::loadDependencies(QString path)
{
  clearDependencies();

  QFile f(path);

  if (f.open(QFile::ReadOnly)) {
    QTextStream s(&f);
    int i,o;

    while (!s.atEnd()) {
      s >> i >> o;

      addDependency(i, o);
    }
  }
}

void CctwTransformer::transformChunkNumber(int chunkId)
{
  CctwDataChunk *inputChunk = m_InputData->readChunk(chunkId);
  QMap<int, CctwDataChunk*> outputChunks;

  if (inputChunk) {

    transformChunkData(chunkId, inputChunk, outputChunks);

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

    m_InputData->releaseChunk(chunkId);
  }
  else {
    printMessage(tr("Could not read chunk: %1").arg(chunkId));
    exit(1);
  }
}

void CctwTransformer::transformChunkData(int chunkId,
                                         CctwDataChunk *inputChunk,
                                         QMap<int, CctwDataChunk*> &outputChunks)
{
#ifndef QT_NO_DEBUG_OUTPUT
  QTime time;
  time.start();
  qDebug("Transforming chunk data: %d", chunkId);
#endif

  CctwCrystalCoordinateTransform transform(m_Application->parameters(),
                                           tr("transform-%1").arg(chunkId), NULL);

  CctwDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = inputChunk->chunkStart();
  CctwIntVector3D chSize  = inputChunk->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  int lastChunkIndex = -1;

  int osx = get_OversampleX();
  int osy = get_OversampleY();
  int osz = get_OversampleZ();

  double osxstp = osx >= 1 ? 1.0/osx : 0;
  double osystp = osy >= 1 ? 1.0/osy : 0;
  double oszstp = osz >= 1 ? 1.0/osz : 0;

  qDebug("input chunk size: %s\n", qPrintable(chSize.toString()));
  qDebug("output chunk size: %s\n", qPrintable(m_OutputData->chunkSize().toString()));
  m_OutputData->setDimensions(CctwIntVector3D(2048,2048,2048));

  for (int z=0; z<chSize.z(); z++) {
    for (int oz=0; oz<osz; oz++) {
      for (int y=0; y<chSize.y(); y++) {
        for (int oy=0; oy<osy; oy++) {
          for (int x=0; x<chSize.x(); x++) {
            CctwIntVector3D iprelat(x,y,z);
            for (int ox=0; ox<osx; ox++) {
              CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x+ox*osxstp, y+oy*osystp, z+oz*oszstp);
              CctwDoubleVector3D xfmcoord = transform.forward(coords);
              CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

              printMessage(tr("m_OutputData: %1").arg(m_OutputData->dimensions().toString()));
              printMessage(tr("pixels:       %1").arg(pixels      .toString()));

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
                else {
                  printMessage("lastChunk not allocated!");
                  exit(1);
                }
              }
            }
          }
        }
      }
    }
  }
  qDebug("Transform chunk data: %i: done. Time %0.3f s",
                              chunkId,       time.elapsed()/1000.0);
}

void CctwTransformer::transformChunkData2(int chunkId,
                                         CctwDataChunk *inputChunk,
                                         QMap<int, CctwChunkedData::MergeDataType*> &outputChunks)
{
  CctwCrystalCoordinateTransform transform(m_Application->parameters(),
                                           tr("transform-%1").arg(chunkId), NULL);

  CctwIntVector3D inputChunkStart = inputChunk->chunkStart();
  CctwIntVector3D inputChunkSize  = inputChunk->chunkSize();
  CctwDoubleVector3D dblStart(inputChunkStart.x(), inputChunkStart.y(), inputChunkStart.z());

  CctwIntVector3D outputChunkCount(16,16,16);
  CctwIntVector3D outputChunkSize(128,128,128);

  int outputChunkId = -1;

  qDebug("input chunk size:  %s\n", qPrintable(inputChunkSize.toString()));
  qDebug("output chunk size: %s\n", qPrintable(outputChunkSize.toString()));

  for (int z=0; z<inputChunkSize.z(); z++) {
    for (int y=0; y<inputChunkSize.y(); y++) {
      for (int x=0; x<inputChunkSize.x(); x++) {
        CctwIntVector3D inputPixel(x, y, z);
        CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x, y, z);
        CctwDoubleVector3D xfmcoord = transform.forward(coords);
        CctwIntVector3D outputPixel(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

        outputChunkId = CctwChunkedData::chunkContaining(outputChunkCount, outputChunkSize, outputPixel);
        CctwChunkedData::MergeDataType *chunk = NULL;
        if (!outputChunks.contains(outputChunkId)) {
          chunk = makeChunk(outputChunkSize);
          outputChunks[outputChunkId] = chunk;
        }

        CctwIntVector3D outputChunkStart = CctwChunkedData::chunkStart(outputChunkSize, outputChunkId);
        CctwIntVector3D inputPixelRelative  = inputPixel  - inputChunkStart;
        CctwIntVector3D outputPixelRelative = outputPixel - outputChunkStart;
        int ix = inputPixelRelative.x(),  iy = inputPixelRelative.y(),  iz = inputPixelRelative.z();
        int ox = outputPixelRelative.x(), oy = outputPixelRelative.y(), oz = outputPixelRelative.z();
        double value = inputChunk->data(ix, iy, iz);
        assignToChunk(outputChunkSize, ox, oy, oz, value, chunk);
      }}}
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

    runDummyTransformChunkNumber(ckidx);
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
  m_InputData->clearDependencies();
  m_OutputData->clearDependencies();
}

void CctwTransformer::addDependency(int f, int t)
{
  m_InputData->addDependency(f, t);
  m_OutputData->addDependency(t, f);
}

void CctwTransformer::projectInput(QString path, int axes)
{
  projectDataset(path, m_InputData, axes);
}

void CctwTransformer::projectOutput(QString path, int axes)
{
  projectDataset(path, m_OutputData, axes);
}

void CctwTransformer::projectDatasetChunk(CctwChunkedData *data, int i, int axes)
{
  if (m_Application && !m_Application->get_Halting()) {
    CctwDataChunk *chunk = data->readChunk(i);

    if (chunk) {
      CctwIntVector3D chStart = chunk->chunkStart();
      CctwIntVector3D chSize  = chunk->chunkSize();

      int cx = chStart.x(),
          cy = chStart.y(),
          cz = chStart.z();

      QcepImageData<double> *imgx = NULL, *imgy = NULL, *imgz = NULL;

      if (axes & 1) {
        imgx = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.y(), chSize.z());
      }

      if (axes & 2) {
        imgy = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.z());
      }

      if (axes & 4) {
        imgz = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.y());
      }

      for (int z=0; z<chSize.z(); z++) {
        for (int y=0; y<chSize.y(); y++) {
          for (int x=0; x<chSize.x(); x++) {
            double data = chunk->data(x,y,z);
            double wgt  = chunk->weight(x,y,z);

            if (wgt != 0) {
              if (imgx) {
                imgx->addValue(y,z,data);
              }

              if (imgy) {
                imgy->addValue(x,z,data);
              }

              if (imgz) {
                imgz->addValue(x,y,data);
              }
            }
          }
        }
      }

      if (axes & 1) {
        QcepMutexLocker lock(__FILE__, __LINE__, &m_LockX);

        if (m_ImageX && imgx) {
          for (int z=0; z<chSize.z(); z++) {
            for (int y=0; y<chSize.y(); y++) {
              m_ImageX->addValue(cy+y, cz+z, imgx->value(y,z));
            }
          }
        }

        delete imgx;
      }

      if (axes & 2) {
        QcepMutexLocker lock(__FILE__, __LINE__, &m_LockY);

        if (m_ImageY && imgy) {
          for (int z=0; z<chSize.z(); z++) {
            for (int x=0; x<chSize.x(); x++) {
              m_ImageY->addValue(cx+x, cz+z, imgy->value(x,z));
            }
          }
        }

        delete imgy;
      }

      if (axes & 4) {
        QcepMutexLocker lock(__FILE__, __LINE__, &m_LockZ);

        if (m_ImageZ && imgz) {
          for (int y=0; y<chSize.y(); y++) {
            for (int x=0; x<chSize.x(); x++) {
               m_ImageZ->addValue(cx+x, cy+y, imgz->value(x,y));
            }
          }
        }

        delete imgz;
      }

      m_InputData->releaseChunk(i);
    }

    m_MergeCounter.fetchAndAddOrdered(-1);
  } else {
    m_MergeCounter.fetchAndStoreOrdered(0);
  }

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwTransformer::projectDataset(QString path, CctwChunkedData *data, int axes)
{
  if (data) {
    if (m_Application) {
      m_Application->waitCompleted();
      m_Application->set_Progress(0);
      m_Application->set_Halting(false);
    }

    QTime startAt;

    startAt.start();

    printMessage("Starting Projection");

    m_MergeCounter.fetchAndStoreOrdered(0);

    QcepImageDataFormatTiff<double> fmt("TIFF");

    CctwIntVector3D dims = data->dimensions();

    int px = axes & 1,
        py = axes & 2,
        pz = axes & 4;

    delete m_ImageX;
    delete m_ImageY;
    delete m_ImageZ;

    if (px) {
      m_ImageX = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.y(), dims.z());
    } else {
      m_ImageX = NULL;
    }

    if (py) {
      m_ImageY = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.z());
    } else {
      m_ImageY = NULL;
    }

    if (pz) {
      m_ImageZ = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.y());
    } else {
      m_ImageZ = NULL;
    }

    int nc = data->chunkCount().volume();

    if (m_Application) {
      m_Application->set_ProgressLimit(nc);
    }

    for (int i=0; i<nc; i++) {
      m_MergeCounter.fetchAndAddOrdered(1);

      if (m_Application) {
        if (m_Application->get_Halting()) break;

        m_Application->addWorkOutstanding(1);
      }

      QtConcurrent::run(this, &CctwTransformer::projectDatasetChunk, data, i, axes);
    }

    while (m_Application && m_MergeCounter.fetchAndAddOrdered(0) > 0) {
      CctwThread::msleep(10);
      m_Application->processEvents();
    }

    if (m_ImageX) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockX);

      fmt.saveFile(path+".x.tif", m_ImageX, false);
      delete m_ImageX;
      m_ImageX = NULL;
    }

    if (m_ImageY) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockY);

      fmt.saveFile(path+".y.tif", m_ImageY, false);
      delete m_ImageY;
      m_ImageY = NULL;
    }

    if (m_ImageZ) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockZ);

      fmt.saveFile(path+".z.tif", m_ImageZ, false);
      delete m_ImageZ;
      m_ImageZ = NULL;
    }

    set_WallTime(startAt.elapsed()/1000.0);

    printMessage(tr("Projection complete after %1 sec").arg(get_WallTime()));
  }
}
