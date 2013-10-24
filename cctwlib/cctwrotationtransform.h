#ifndef CCTWROTATIONTRANSFORM_H
#define CCTWROTATIONTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwRotationTransform : public CctwTransformInterface
{
#ifdef USE_QT
  Q_OBJECT
public:
  CctwRotationTransform(QObject *parent);
#else
public:
  CctwRotationTransform();
#endif

public:
//  static CctwRotationTransform* createNew(int argc, char *argv[]);
//                                          // Allocate and return a new rotation transformation object
//                                          // based on the command line parameters passed in argc and argv

  virtual bool hasInverse() const;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWROTATIONTRANSFORM_H