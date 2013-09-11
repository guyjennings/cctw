#include "cctwtransforminterface.h"

#ifdef USE_QT
CctwTransformInterface::CctwTransformInterface(QObject *parent) :
  CctwqtObject(parent)
{
}
#else
CctwTransformInterface::CctwTransformInterface()
{
}
#endif
