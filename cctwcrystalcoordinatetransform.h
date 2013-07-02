#ifndef CCTWCRYSTALCOORDINATETRANSFORM_H
#define CCTWCRYSTALCOORDINATETRANSFORM_H

#include "cctwtransforminterface.h"

class CctwCrystalCoordinateTransform : public CctwTransformInterface
{
public:
  CctwCrystalCoordinateTransform();

  virtual CctwVector3D forward(CctwVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwVector3D inverse(CctwVector3D q);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWCRYSTALCOORDINATETRANSFORM_H
