#include "cctwqttransformer.h"

CctwqtTransformer::CctwqtTransformer
  (CctwInputDataInterface *input,
   CctwOutputDataInterface *output,
   CctwTransformInterface *xform,
   int osx, int osy, int osz,
   QObject *parent) :
  CctwqtObject(parent),
  CctwTransformer(input, output, xform, osx, osy, osz, 0)
{
}
