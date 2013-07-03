#ifndef CCTWCRYSTALCOORDINATETRANSFORM_H
#define CCTWCRYSTALCOORDINATETRANSFORM_H

#include "cctwtransforminterface.h"

class CctwCrystalCoordinateTransform : public CctwTransformInterface
{
public:
  CctwCrystalCoordinateTransform();

public:
  static CctwCrystalCoordinateTransform* createNew(int argc, char *argv[]);
                                          // Allocate and return a new crystal coordinate transformation object
                                          // based on the command line parameters passed in argc and argv

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWCRYSTALCOORDINATETRANSFORM_H
