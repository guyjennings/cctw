#ifndef CCTWROTATIONTRANSFORM_H
#define CCTWROTATIONTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwRotationTransform : public CctwTransformInterface
{
public:
  CctwRotationTransform();

public:
  static CctwRotationTransform* createNew(int argc, char *argv[]);
                                          // Allocate and return a new rotation transformation object
                                          // based on the command line parameters passed in argc and argv

  virtual CctwVector3D<double> forward(CctwVector3D<double> a);
                                          // Transform from input to output space coordinates
  virtual CctwVector3D<double> inverse(CctwVector3D<double> q);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWROTATIONTRANSFORM_H
