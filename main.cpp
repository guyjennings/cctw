#include <stdio.h>
#include "cctwswiftinputdata.h"
#include "cctwswiftoutputdata.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwrotationtransform.h"
#include "cctwcompoundtransform.h"
#include "cctwtransformer.h"
#include "cctwoutputchunkindex.h"

#ifdef USE_QT
#include "cctwqtapplication.h"
#endif

int main(int argc, char *argv[])
{
#ifdef USE_QT
  CctwqtApplication *app = new CctwqtApplication(argc, argv);

  app->exec();
#else
  CctwSwiftInputData  *inputData  = CctwSwiftInputData::createNew(argc, argv);
  CctwSwiftOutputData *outputData = CctwSwiftOutputData::createNew(argc, argv);
  CctwCrystalCoordinateTransform *xtalTransform = CctwCrystalCoordinateTransform::createNew(argc, argv);
  CctwRotationTransform          *rotnTransform = CctwRotationTransform::createNew(argc, argv);
  CctwCompoundTransform          *cmpdTransform = NULL;

  if (xtalTransform && rotnTransform) {
    cmpdTransform = new CctwCompoundTransform(xtalTransform, rotnTransform);
  }

  CctwTransformInterface *transform = NULL;

  if (cmpdTransform) {
    printf("Compound transform specified\n");
    transform = cmpdTransform;
  } else if (xtalTransform) {
    printf("Crystal coordinate transform specified\n");
    transform = xtalTransform;
  } else if (rotnTransform) {
    printf("Rotation transform specified\n");
    transform = rotnTransform;
  } else {
    printf("No transformation specified\n");
  }

  CctwTransformer *transformer = NULL;

  if (transform) {
    transformer = CctwTransformer::createNew(argc, argv, inputData, outputData, transform);
  }

  CctwOutputChunkIndex chunk = CctwOutputChunkIndex::createNew(argc, argv);

  if (transformer) {
    printf("Transformation of chunk [%d,%d,%d] starting\n", chunk.nx(), chunk.ny(), chunk.nz());
    transformer->transformChunk(chunk.nx(), chunk.ny(), chunk.nz());
    printf("Transformation finished\n");
  } else {
    printf("No transformation performed\n");
  }
#endif
}
