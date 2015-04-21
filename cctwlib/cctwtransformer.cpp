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
#include <QFile>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QUrlQuery>
#endif

#ifdef WANT_ANALYSIS_COMMANDS
#include "qcepimagedataformattiff.h"
#include "qcepimagedata.h"
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
  m_WeightX(NULL),
  m_WeightY(NULL),
  m_WeightZ(NULL),
  m_WallTime(QcepSettingsSaverWPtr(), this, "wallTime", 0, "Wall Time of last command"),
  m_BlocksLimit(m_Application->saver(), this, "blocksLimit", 1000, "Blocks Limit"),
  m_TransformOptions(m_Application->saver(), this, "transformOptions", 0, "Transform Options"),
  m_OversampleX(m_Application->saver(), this, "oversampleX", 1, "Oversampling along X"),
  m_OversampleY(m_Application->saver(), this, "oversampleY", 1, "Oversampling along Y"),
  m_OversampleZ(m_Application->saver(), this, "oversampleZ", 1, "Oversampling along Z"),
  m_ProjectX(m_Application->saver(), this, "projectX", true, "Project along X"),
  m_ProjectY(m_Application->saver(), this, "projectY", true, "Project along Y"),
  m_ProjectZ(m_Application->saver(), this, "projectZ", true, "Project along Z"),
  m_ProjectDestination(m_Application->saver(), this, "projectDestination", "", "Output path for projected images"),
  m_Normalization(m_Application->saver(), this, "normalization", 1, "Normalize output data?"),
  m_Compression(m_Application->saver(), this, "compression", 2, "Compression level for output data"),
  m_Subset(QcepSettingsSaverWPtr(), this, "subset", "", "Subset specifier"),
  m_UseDependencies(QcepSettingsSaverWPtr(), this, "useDependencies", 0, "Use dependencies in transform")
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
  }

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
    int nchunks = m_InputData->chunkCount().volume();

//    m_Application->printMessage(tr("Chunk %1/%2 transform complete").arg(n).arg(nchunks));
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
  clearDependencies(1);

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

bool CctwTransformer::parseSubset(CctwChunkedData *data)
{
  if (data == NULL) data = m_InputData;

  QString subset = get_Subset();

  if (subset.length() == 0) {
    m_SubsetStart = CctwIntVector3D(0,0,0);
    m_SubsetEnd   = data->chunkCount();

    return true;
  } else {
    m_SubsetStart = CctwIntVector3D(0,0,0);
    m_SubsetEnd   = CctwIntVector3D(0,0,0);

    CctwIntVector3D chunks = data->chunkCount();

    QRegExp r("(\\d+)/(\\d+)");

    if (r.exactMatch(subset)) {
      int index = r.cap(1).toInt();
      int nsubs = r.cap(2).toInt();
      int nbest = 0, bestdx = 0, bestdy = 0, bestdz = 0;
      int bestnx = 0, bestny = 0, bestnz = 0;

      if (m_Application->get_Verbosity() >= 2) {
        printMessage(tr("Subset matched : index = %1 of %2").arg(index).arg(nsubs));
      }

      int szx = chunks.x(), szy = chunks.y(), szz = chunks.z();

      if (m_Application->get_Verbosity() >= 2) {
        printMessage(tr("Size %1,%2,%3 - total %4")
                     .arg(szx).arg(szy).arg(szz).arg(szx*szy*szz));
      }

      for (int dx=1; dx<=szx; dx++) {
        int nx = (szx + dx - 1)/dx;

        if (m_Application->get_Verbosity() >= 3) {
        printMessage(tr("DX:%1:NX:%2").arg(dx).arg(nx));
    }

        if (nx > 0) {

          for (int dy=1; dy <= szy; dy++) {
            int ny = (szy + dy - 1)/dy;

            if (m_Application->get_Verbosity() >= 3) {
              printMessage(tr("DY:%1:NY:%2").arg(dy).arg(ny));
            }

            if (ny > 0) {
              for (int dz=1; dz <= szz; dz++) {
                int nz = (szz + dz - 1)/dz;

                if (m_Application->get_Verbosity() >= 3) {
                  printMessage(tr("DZ:%1:NZ:%2").arg(dz).arg(nz));
                }

                if (nz > 0) {
                  int ntot = nx*ny*nz;

                  if (m_Application->get_Verbosity() >= 3) {
                    printMessage(tr("SZ:%1,%2,%3; NUM:%4,%5,%6; NSUB:%7; NTOT:%8; NBEST:%9")
                                 .arg(dx).arg(dy).arg(dz)
                                 .arg(nx).arg(ny).arg(nz)
                                 .arg(nsubs).arg(ntot).arg(nbest));
                  }

                  if (ntot <= nsubs) {
                    if (ntot > nbest) {
                      nbest  = ntot;
                      bestdx = dx; bestnx = nx;
                      bestdy = dy; bestny = ny;
                      bestdz = dz; bestnz = nz;
                    } else if (ntot == nbest) {
                      int bestmax = qMax(qMax(bestdx,bestdy),bestdz);
                      int newmax  = qMax(qMax(dx,dy),dz);

                      if (newmax < bestmax) {
                        bestdx = dx; bestnx = nx;
                        bestdy = dy; bestny = ny;
                        bestdz = dz; bestnz = nz;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

      if (nbest > 0) {
        if (m_Application->get_Verbosity() >= 2) {
          printMessage(tr("Best subdivision found: %1,%2,%3 - total %4")
                       .arg(bestdx).arg(bestdy).arg(bestdz).arg(bestnx*bestny*bestnz));
        }

        if ((index >= nbest) || (index < 0)) {
          if (m_Application->get_Verbosity() >= 2) {
            printMessage(tr("Skipping subset %1 of %2").arg(index).arg(nbest));
          }
          return false;
        } else {
          //          for (int index=0; index<nbest; index++) {
          int xstride = 1,
              ystride = bestnx,
              zstride = bestnx*bestny;

          int n = index;

          int z = n / zstride;

          n %= zstride;

          int y = n / ystride;

          n %= ystride;

          int x = n / xstride;

          int x0 = x*bestdx, x1 = x0+bestdx;
          int y0 = y*bestdy, y1 = y0+bestdy;
          int z0 = z*bestdz, z1 = z0+bestdz;

          if (x1 > chunks.x()) x1 = chunks.x();
          if (y1 > chunks.y()) y1 = chunks.y();
          if (z1 > chunks.z()) z1 = chunks.z();

          if (m_Application->get_Verbosity() >= 2) {
            printMessage(tr("Subset %1 : [%2..%3) [%4..%5) [%6..%7)")
                         .arg(index)
                         .arg(x0).arg(x1).arg(y0).arg(y1).arg(z0).arg(z1));
          }

          m_SubsetStart = CctwIntVector3D(x0,y0,z0);
          m_SubsetEnd   = CctwIntVector3D(x1,y1,z1);

          return true;
        }
      }
    } else {
      printMessage("Subset mismatch");
      return false;
    }
  }

  return false;
}

void CctwTransformer::transformChunkNumber(int chunkId)
{
  CctwDataChunk *inputChunk = m_InputData->readChunk(chunkId);
  QMap<int, CctwDataChunk*> outputChunks;

  if (inputChunk) {

    transformChunkData(chunkId, inputChunk, outputChunks);

    inputChunk->deallocateData();
    inputChunk->deallocateWeights();

//    printMessage(tr("Need to merge %1 output chunks from input chunk [%2]")
//                       .arg(outputChunks.count())
//                       .arg(chunkId));

    if (inputChunk->dependencyCount() && inputChunk->dependencyCount() != outputChunks.count()) {
      printMessage(tr("Discrepancy between numbers of merged chunks : dependencyCount() = %1, chunks.count() = %2")
                   .arg(inputChunk->dependencyCount()).arg(outputChunks.count()));
    }

    foreach(CctwDataChunk *outputChunk, outputChunks) {
      m_OutputData->mergeChunk(outputChunk);

      delete outputChunk;
    }

    m_InputData->releaseChunkData(chunkId);
  } else {
    printMessage(tr("Could not read chunk: %1").arg(chunkId));
//    exit(1);
  }
}

void CctwTransformer::transformChunkData(int chunkId,
                                         CctwDataChunk *inputChunk,
                                         QMap<int, CctwDataChunk*> &outputChunks)
{
  QTime time;
  time.start();

  if (m_Application->get_Verbosity() >= 3) {
    printMessage(tr("Transforming chunk data: %1").arg(chunkId));
  }

  QcepDoubleVector anglesvec = m_InputData->get_Angles();
  QcepDoubleVector weightsvec= m_InputData->get_Weights();
  QcepIntVector    maskvec   = m_InputData->get_Mask();
  CctwIntVector3D  dims      = m_InputData->dimensions();

  int nMask = maskvec.count();
  int nAngs = anglesvec.count();
  int nWgts = weightsvec.count();

  const int    *mask = maskvec.constData();
  const double *angs = anglesvec.constData();
  const double *wgts = weightsvec.constData();

  if (nMask <= 0) mask = NULL;
  if (nAngs <= 0) angs = NULL;
  if (nWgts <= 0) wgts = NULL;

  CctwCrystalCoordinateTransform transform(m_Application->parameters(),
                                           tr("transform-%1").arg(chunkId),
                                           angs,
                                           NULL);

  CctwDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = inputChunk->chunkStart();
  CctwIntVector3D chSize  = inputChunk->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  int lastChunkIndex = -1;

  int osx = get_OversampleX();
  int osy = get_OversampleY();
  int osz = get_OversampleZ();

  int nused = 0, nskipped = 0;

  double osxstp = osx >= 1 ? 1.0/osx : 0;
  double osystp = osy >= 1 ? 1.0/osy : 0;
  double oszstp = osz >= 1 ? 1.0/osz : 0;

  for (int z=0; z<chSize.z(); z++) {
    double angle = anglesvec.value(z+chStart.z());
    double weight = weightsvec.value(z+chStart.z());

    if ((angs==NULL || angle==angle) &&
        (wgts==NULL || weight > 0)) {

      if (wgts==NULL) weight = 1;

      for (int oz=0; oz<osz; oz++) {
        for (int y=0; y<chSize.y(); y++) {
          for (int oy=0; oy<osy; oy++) {
            for (int x=0; x<chSize.x(); x++) {
              CctwIntVector3D globalpix(chStart + CctwIntVector3D(x,y,z));


              if (mask == NULL || mask[(globalpix.y())*dims.x() + globalpix.x()] == 0) {
                nused++;
                CctwIntVector3D iprelat(x,y,z);
                for (int ox=0; ox<osx; ox++) {
                  CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x+ox*osxstp, y+oy*osystp, z+oz*oszstp);
                  CctwDoubleVector3D xfmcoord = transform.forward(coords);
                  CctwIntVector3D pixels(xfmcoord);

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
                      double iwgt = inputChunk->weight(ix, iy, iz)*weight;

                      if (iwgt != 0) {
                        lastChunk->setData(ox, oy, oz, oval+ival);
                        lastChunk->setWeight(ox, oy, oz, owgt+iwgt);
                      }
                    }
                  }
                }
              } else {
                nskipped++;
              }
            }
          }
        }
      }
    }
  }

  if (m_Application->get_Verbosity() >= 3) {
    printMessage(tr("Transform chunk data: %1: done. Time %2 s, %3 output chunks, %4 allocated, used %5, skipped %6")
                 .arg(chunkId)
                 .arg(time.elapsed()/1000.0,5)
                 .arg(outputChunks.count())
                 .arg(CctwDataChunk::allocatedChunkCount())
                 .arg(nused)
                 .arg(nskipped)
                 );
  }
}

void CctwTransformer::transform()
{
  QVector < QFuture < void > > futures;

  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
    m_Application->set_ExitStatus(0);
  }

  QTime startAt;

  startAt.start();

  if (m_Application->get_Verbosity() >= 1) {
    printMessage("Starting Transform");
  }

  if (m_InputData  -> beginTransform(true,  get_TransformOptions())) {
    if (m_OutputData -> beginTransform(false, get_TransformOptions())) {

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

      if (m_Application->get_Verbosity() >= 2) {
        printMessage(tr("%1 chunks of input data needed").arg(inputChunks.count()));
      }

      if ((get_TransformOptions() & 2)) {
        if (m_Application->get_Verbosity() >= 2) {
          printMessage("Sorting input chunk list into input order");
        }

        qSort(inputChunks.begin(), inputChunks.end());
      }

      if (m_Application) {
        m_Application->set_ProgressLimit(inputChunks.count());
      }

      foreach(int ckidx, inputChunks) {
        if (m_Application) {
          m_Application->addWorkOutstanding(1);
        }

        if ((get_TransformOptions() & 1) == 0) {
          futures.append(
                QtConcurrent::run(this, &CctwTransformer::runTransformChunkNumber, ckidx));
        } else {
          runTransformChunkNumber(ckidx);
        }
      }

      foreach (QFuture<void> f, futures) {
        f.waitForFinished();
        if (m_Application) {
          m_Application->processEvents();
        }
      }

      set_WallTime(startAt.elapsed()/1000.0);

      m_OutputData -> endTransform();
    } else {
      printMessage("Failed to open output data");
      m_Application->set_ExitStatus(1);
    }

    m_InputData  -> endTransform();
  } else {
    printMessage("Failed to open input data");
    m_Application->set_ExitStatus(1);
  }

  if (m_Application->get_Verbosity() >= 1) {
    printMessage(tr("Transform complete after %1 sec, %2 chunks still allocated")
                 .arg(get_WallTime())
                 .arg(CctwDataChunk::allocatedChunkCount()));
  }
}

void CctwTransformer::simpleTransform()
{
  QVector < QFuture < void > > futures;

  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
    m_Application->set_ExitStatus(0);
  }

  int msec;

  if (m_InputData  -> beginTransform(true,  0)) {
    if (m_OutputData -> beginTransform(false, 0)) {

      parseSubset();

      CctwIntVector3D chunkStart = m_SubsetStart;
      CctwIntVector3D chunkEnd   = m_SubsetEnd;
      CctwIntVector3D nChunks    = chunkEnd - chunkStart;

      if (m_Application) {
        m_Application->set_ProgressLimit(nChunks.volume());
      }

      QTime startAt;

      startAt.start();

      if (m_Application->get_Verbosity() >= 1) {
        printMessage("Starting Transform");
      }

      if (m_Application->get_Verbosity() >= 2) {
        printMessage(tr("Input Dimensions %1, Output Dimensions %2")
                     .arg(m_InputData->dimensions().toString())
                     .arg(m_OutputData->dimensions().toString()));
        printMessage(tr("Input Chunk Size %1, Output Chunk Size %2")
                     .arg(m_InputData->chunkSize().toString())
                     .arg(m_OutputData->chunkSize().toString()));
        printMessage(tr("Input Chunk Count %1, Output Chunk Count %2")
                     .arg(m_InputData->chunkCount().toString())
                     .arg(m_OutputData->chunkCount().toString()));
        printMessage(tr("Input HDF Chunk Size %1, Output HDF Chunk Size %2")
                     .arg(m_InputData->get_HDFChunkSize().toString())
                     .arg(m_OutputData->get_HDFChunkSize().toString()));
      }

      for (int z=chunkStart.z(); z<chunkEnd.z(); z++) {
        for (int y=chunkStart.y(); y<chunkEnd.y(); y++) {
          for (int x=chunkStart.x(); x<chunkEnd.x(); x++) {
            if (m_Application && m_Application->get_Halting()) {
              goto abort;
            } else {
              CctwIntVector3D idx(x,y,z);

              int n = m_InputData->chunkNumberFromIndex(idx);

              if (m_Application) {
                m_Application->addWorkOutstanding(1);
              }

              futures.append(
                    QtConcurrent::run(this, &CctwTransformer::runTransformChunkNumber, n));
            }
          }
        }
      }

abort:
      foreach (QFuture<void> f, futures) {
        f.waitForFinished();
        if (m_Application) {
          m_Application->processEvents();
        }
      }

      msec = startAt.elapsed();

      m_OutputData -> flushOutputFile();

      m_OutputData -> endTransform();
    } else {
      printMessage("Failed to open output data");
      m_Application->set_ExitStatus(1);
    }

    m_InputData  -> endTransform();
  } else {
    printMessage("Failed to open input data");
    m_Application->set_ExitStatus(1);
  }

  if (m_Application->get_Verbosity() >= 1) {
    printMessage(tr("Transform complete after %1 msec, %2 chunks still allocated")
                 .arg(msec)
                 .arg(CctwDataChunk::allocatedChunkCount()));
  }
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
  QcepDoubleVector anglesvec = m_InputData->get_Angles();
  double *angs = (anglesvec.count()<=0 ? NULL : anglesvec.data());

  CctwCrystalCoordinateTransform transform(m_Application->parameters(), tr("transform-%1").arg(n), angs, NULL);

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
            CctwIntVector3D pixels(xfmcoord);

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

void CctwTransformer::clearDependencies(int use)
{
  m_InputData->clearDependencies();
  m_OutputData->clearDependencies();

  set_UseDependencies(use);
}

void CctwTransformer::addDependency(int f, int t)
{
  m_InputData->addDependency(f, t);
  m_OutputData->addDependency(t, f);
}

#ifdef WANT_ANALYSIS_COMMANDS

void CctwTransformer::inputProject(QString path, int axes)
{
  projectDataset(path, m_InputData, axes);
}

void CctwTransformer::outputProject(QString path, int axes)
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
      QcepImageData<double> *wgtx = NULL, *wgty = NULL, *wgtz = NULL;

      if (axes & 1) {
        imgx = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.y(), chSize.z());
        wgtx = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.y(), chSize.z());
      }

      if (axes & 2) {
        imgy = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.z());
        wgty = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.z());
      }

      if (axes & 4) {
        imgz = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.y());
        wgtz = new QcepImageData<double>(QcepSettingsSaverWPtr(), chSize.x(), chSize.y());
      }

      double mindata = chunk->data(0,0,0);
      double maxdata = mindata;
      double minwgt  = chunk->weight(0,0,0);
      double maxwgt  = minwgt;

      for (int z=0; z<chSize.z(); z++) {
        for (int y=0; y<chSize.y(); y++) {
          for (int x=0; x<chSize.x(); x++) {
            double data = chunk->data(x,y,z);
            double wgt  = chunk->weight(x,y,z);

            if (data < mindata) mindata = data;
            if (data > maxdata) maxdata = data;
            if (wgt < minwgt) minwgt = wgt;
            if (wgt > maxwgt) maxwgt = wgt;

            if (wgt > 0) {
              if (imgx) {
                imgx->addValue(y,z,data);
                wgtx->addValue(y,z,wgt);
              }

              if (imgy) {
                imgy->addValue(x,z,data);
                wgty->addValue(x,z,wgt);
              }

              if (imgz) {
                imgz->addValue(x,y,data);
                wgtz->addValue(x,y,wgt);
              }
            } else if (data != 0){
              printMessage(tr("Wgt: %1, Data %2").arg(wgt).arg(data));
            }
          }
        }
      }

      {
        QcepMutexLocker lock(__FILE__, __LINE__, &m_LockX);

        if (minwgt < m_MinWeight) m_MinWeight = minwgt;
        if (maxwgt > m_MaxWeight) m_MaxWeight = maxwgt;
        if (mindata < m_MinData)  m_MinData = mindata;
        if (maxdata > m_MaxData)  m_MaxData = maxdata;
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

        if (m_WeightX && wgtx) {
          for (int z=0; z<chSize.z(); z++) {
            for (int y=0; y<chSize.y(); y++) {
              m_WeightX->addValue(cy+y, cz+z, wgtx->value(y,z));
            }
          }
        }

        delete imgx;
        delete wgtx;
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

        if (m_WeightY && wgty) {
          for (int z=0; z<chSize.z(); z++) {
            for (int x=0; x<chSize.x(); x++) {
              m_WeightY->addValue(cx+x, cz+z, wgty->value(x,z));
            }
          }
        }

        delete imgy;
        delete wgty;
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

        if (m_WeightZ && wgtz) {
          for (int y=0; y<chSize.y(); y++) {
            for (int x=0; x<chSize.x(); x++) {
               m_WeightZ->addValue(cx+x, cy+y, wgtz->value(x,y));
            }
          }
        }

        delete imgz;
        delete wgtz;
      }

      data->releaseChunkData(i);
    }
  }

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwTransformer::projectDataset(QString path, CctwChunkedData *data, int axes)
{
  if (data  && data->openInputFile()) {
    QVector < QFuture < void > > futures;

    if (m_Application) {
      m_Application->waitCompleted();
      m_Application->set_Progress(0);
      m_Application->set_Halting(false);
    }

    QTime startAt;

    startAt.start();

    printMessage("Starting Projection");

    QcepImageDataFormatTiff<double> fmt("TIFF");

    CctwIntVector3D dims = data->dimensions();

    m_MinData = INFINITY;
    m_MaxData = -INFINITY;
    m_MinWeight = INFINITY;
    m_MaxWeight = -INFINITY;

    int px = axes & 1,
        py = axes & 2,
        pz = axes & 4;

    delete m_ImageX;
    delete m_WeightX;
    delete m_ImageY;
    delete m_WeightY;
    delete m_ImageZ;
    delete m_WeightZ;

    if (px) {
      m_ImageX = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.y(), dims.z());
      m_WeightX = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.y(), dims.z());
    } else {
      m_ImageX = NULL;
      m_WeightX = NULL;
    }

    if (py) {
      m_ImageY = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.z());
      m_WeightY = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.z());
    } else {
      m_ImageY = NULL;
      m_WeightY = NULL;
    }

    if (pz) {
      m_ImageZ = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.y());
      m_WeightZ = new QcepImageData<double>(QcepSettingsSaverWPtr(), dims.x(), dims.y());
    } else {
      m_ImageZ = NULL;
      m_WeightZ = NULL;
    }

//    int nc = data->chunkCount().volume();

//    if (m_Application) {
//      m_Application->set_ProgressLimit(nc);
//    }

//    for (int i=0; i<nc; i++) {
//      if (m_Application) {
//        if (m_Application->get_Halting()) break;

//        m_Application->addWorkOutstanding(1);
//      }

//      futures.append(
//            QtConcurrent::run(this, &CctwTransformer::projectDatasetChunk, data, i, axes));
//    }

    parseSubset(data);

    CctwIntVector3D chunkStart = m_SubsetStart;
    CctwIntVector3D chunkEnd   = m_SubsetEnd;
    CctwIntVector3D nChunks     = chunkEnd - chunkStart;

    if (m_Application) {
      m_Application->set_ProgressLimit(nChunks.volume());
    }

    for (int z=chunkStart.z(); z<chunkEnd.z(); z++) {
      for (int y=chunkStart.y(); y<chunkEnd.y(); y++) {
        for (int x=chunkStart.x(); x<chunkEnd.x(); x++) {
          if (m_Application && m_Application->get_Halting()) {
            goto abort;
          } else {
            CctwIntVector3D idx(x,y,z);

            int n = data->chunkNumberFromIndex(idx);

            if (m_Application) {
              m_Application->addWorkOutstanding(1);
            }

            futures.append(
                  QtConcurrent::run(this, &CctwTransformer::projectDatasetChunk, data, n, axes));
          }
        }
      }
    }

abort:
    foreach (QFuture<void> f, futures) {
      f.waitForFinished();
      if (m_Application) {
        m_Application->processEvents();
      }
    }

    if (m_ImageX) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockX);

      if (m_WeightX) {
        for (int y=0; y<(m_ImageX->get_Height()); y++) {
          for (int x=0; x<(m_ImageX->get_Width()); x++) {
            double val = m_ImageX->value(x,y);
            double wgt = m_WeightX->value(x,y);

            if (wgt > 0) {
              m_ImageX->setValue(x,y, val/wgt);
            } else {
              m_ImageX->setValue(x,y, val);
            }
          }
        }
      }

      fmt.saveFile(path+".x.tif", m_ImageX, false);
      delete m_ImageX;
      delete m_WeightX;
      m_ImageX = NULL;
      m_WeightX = NULL;
    }

    if (m_ImageY) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockY);

      if (m_WeightY) {
        for (int y=0; y<(m_ImageY->get_Height()); y++) {
          for (int x=0; x<(m_ImageY->get_Width()); x++) {
            double val = m_ImageY->value(x,y);
            double wgt = m_WeightY->value(x,y);

            if (wgt > 0) {
              m_ImageY->setValue(x,y, val/wgt);
            } else {
              m_ImageY->setValue(x,y, val);
            }
          }
        }
      }

      fmt.saveFile(path+".y.tif", m_ImageY, false);
      delete m_ImageY;
      delete m_WeightY;
      m_ImageY = NULL;
      m_WeightY = NULL;
    }

    if (m_ImageZ) {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_LockZ);

      if (m_WeightZ) {
        for (int y=0; y<(m_ImageZ->get_Height()); y++) {
          for (int x=0; x<(m_ImageZ->get_Width()); x++) {
            double val = m_ImageZ->value(x,y);
            double wgt = m_WeightZ->value(x,y);

            if (wgt > 0) {
              m_ImageZ->setValue(x,y, val/wgt);
            } else {
              m_ImageZ->setValue(x,y, val);
            }
          }
        }
      }

      fmt.saveFile(path+".z.tif", m_ImageZ, false);
      delete m_ImageZ;
      delete m_WeightZ;
      m_ImageZ = NULL;
      m_WeightZ = NULL;
    }

    set_WallTime(startAt.elapsed()/1000.0);

    printMessage(tr("Projection complete after %1 sec, Min %2, Max %3, MinW %4, MaxW %5")
                 .arg(get_WallTime()).arg(m_MinData).arg(m_MaxData).arg(m_MinWeight).arg(m_MaxWeight));
  }
}

#endif
