#ifndef CCTWTRANSFORMINTERFACE_H
#define CCTWTRANSFORMINTERFACE_H

#include "cctwvector3d.h"

class CctwTransformInterface
{
public:
  CctwTransformInterface();

  virtual bool hasInverse() const = 0;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D) = 0;
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q) = 0;
                                          // Back transform from output to input space coordinates
};

#endif // CCTWTRANSFORMINTERFACE_H
