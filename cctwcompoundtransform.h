#ifndef CCTWCOMPOUNDTRANSFORM_H
#define CCTWCOMPOUNDTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwCompoundTransform : public CctwTransformInterface
{
public:
  CctwCompoundTransform(CctwTransformInterface *t1, CctwTransformInterface *t2);

public:
  virtual CctwVector3D<double> forward(CctwVector3D<double> a);
                                          // Transform from input to output space coordinates
  virtual CctwVector3D<double> inverse(CctwVector3D<double> q);
                                          // Back transform from output to input space coordinates

private:
  CctwTransformInterface *m_Transform1;
  CctwTransformInterface *m_Transform2;
};

#endif // CCTWCOMPOUNDTRANSFORM_H
