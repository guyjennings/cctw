#include "cctwrotationtransform.h"

#ifdef USE_QT
CctwRotationTransform::CctwRotationTransform(QObject *parent) :
  CctwTransformInterface(parent)
{
}
#else
CctwRotationTransform::CctwRotationTransform() :
  CctwTransformInterface()
{
}
#endif

bool CctwRotationTransform::hasInverse() const
{
  return true;
}

//CctwRotationTransform *CctwRotationTransform::createNew(int argc, char *argv[])
//{
//  return new CctwRotationTransform();
//}

CctwDoubleVector3D CctwRotationTransform::forward(CctwDoubleVector3D a)
{
  return a;
}

CctwDoubleVector3D CctwRotationTransform::inverse(CctwDoubleVector3D q)
{
  return q;
}
