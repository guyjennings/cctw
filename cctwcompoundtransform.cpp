#include "cctwcompoundtransform.h"

CctwCompoundTransform::CctwCompoundTransform(CctwTransformInterface *t1, CctwTransformInterface *t2)
  : CctwTransformInterface(),
    m_Transform1(t1),
    m_Transform2(t2)
{
}

CctwDoubleVector3D CctwCompoundTransform::forward(CctwDoubleVector3D a)
{
  CctwVector3D<double> t1 = m_Transform1->forward(a);

  return m_Transform2->forward(t1);
}

CctwDoubleVector3D CctwCompoundTransform::inverse(CctwDoubleVector3D q)
{
  CctwVector3D<double> i1 = m_Transform2->inverse(q);

  return m_Transform1->inverse(i1);
}
