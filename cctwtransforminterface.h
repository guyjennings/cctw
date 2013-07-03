#ifndef CCTWTRANSFORMINTERFACE_H
#define CCTWTRANSFORMINTERFACE_H

#include "cctwvector3d.h"

class CctwTransformInterface
{
public:
  CctwTransformInterface();

  virtual CctwVector3D<double> forward(CctwVector3D<double> a) = 0;
                                          // Transform from input to output space coordinates
  virtual CctwVector3D<double> inverse(CctwVector3D<double> q) = 0;
                                          // Back transform from output to input space coordinates
};

#endif // CCTWTRANSFORMINTERFACE_H
