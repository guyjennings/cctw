#include "cctwqttransformer.h"
#include <QtConcurrentRun>
#include "cctwqtdatachunk.h"

CctwqtTransformer::CctwqtTransformer
  (CctwInputDataInterface *input,
   CctwOutputDataInterface *output,
   CctwTransformInterface *xform,
   int osx, int osy, int osz,
   QObject *parent) :
  CctwTransformer(input, output, xform, osx, osy, osz, 0, parent)
{
}

void CctwqtTransformer::transformChunk(int nx, int ny, int nz)
{
}

void CctwqtTransformer::transformChunkNumber(int n)
{
  CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);
  CctwIntVector3D lastChunkIndex(-1, -1, -1);
  CctwqtDataChunk *inputChunk = m_InputData->chunk(idx);
  CctwqtDataChunk *lastChunk = NULL;

  CctwIntVector3D chStart = m_InputData->chunkStart(idx);
  CctwIntVector3D chSize  = m_InputData->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  if (inputChunk) {
    QMap<CctwIntVector3D, CctwqtDataChunk*> outputChunks;

    inputChunk->readData();
    inputChunk->readWeights();

    for (int z=0; z<chSize.z(); z++) {
      for (int y=0; y<chSize.y(); y++) {
        for (int x=0; x<chSize.x(); x++) {
          CctwIntVector3D iprelat(x,y,z);
          CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x,y,z);
          CctwDoubleVector3D xfmcoord = m_Transform->forward(coords);
          CctwIntVector3D pixels(xfmcoord.x(), xfmcoord.y(), xfmcoord.z());

          if (m_OutputData->containsPixel(pixels)) {
            CctwIntVector3D opchunk = m_OutputData->chunkIndex(pixels);
            CctwIntVector3D oprelat = pixels - m_OutputData->chunkStart(opchunk);

            if (opchunk != lastChunkIndex) {
              lastChunkIndex = opchunk;

              if (!outputChunks.contains(lastChunkIndex)) {
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
              double iwgt = inputChunk->data(iprelat);

              if (iwgt > 0) {
                lastChunk->setData(oprelat, oval+ival*iwgt);
                lastChunk->setWeight(oprelat, owgt+iwgt);
              }
            }
          }
        }
      }
    }

    inputChunk->deallocateData();
    inputChunk->deallocateWeights();

    printMessage(tr("Need to merge %1 output chunks from [%2,%3,%4]")
                 .arg(outputChunks.count())
                 .arg(idx.x()).arg(idx.y()).arg(idx.z()));

    foreach(CctwqtDataChunk *outputChunk, outputChunks) {
  //    m_OutputData->mergeChunk(outputChunk);

      delete outputChunk;
    }
  }
}

void CctwqtTransformer::transform()
{
//  m_InputData->clearFlags();
//  m_OutputData->clearFlags();

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

  foreach(int ckidx, inputChunks) {
    QtConcurrent::run(this, &CctwqtTransformer::transformChunkNumber, ckidx);
  }
}
