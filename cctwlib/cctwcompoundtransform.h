#ifndef CCTWCOMPOUNDTRANSFORM_H
#define CCTWCOMPOUNDTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwCompoundTransform : public CctwTransformInterface
{
#ifdef USE_QT
  Q_OBJECT
public:
  CctwCompoundTransform(CctwTransformInterface *t1, CctwTransformInterface *t2, QObject *parent);
#else
public:
  CctwCompoundTransform(CctwTransformInterface *t1, CctwTransformInterface *t2);
#endif

public:
  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q);
                                          // Back transform from output to input space coordinates

private:
  CctwTransformInterface *m_Transform1;
  CctwTransformInterface *m_Transform2;
};

#endif // CCTWCOMPOUNDTRANSFORM_H