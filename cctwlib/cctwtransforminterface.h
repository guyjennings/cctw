#ifndef CCTWTRANSFORMINTERFACE_H
#define CCTWTRANSFORMINTERFACE_H

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#include "cctwvector3d.h"

#ifdef USE_QT
class CctwTransformInterface : public CctwqtObject
{
  Q_OBJECT
public:
  CctwTransformInterface(QObject *parent);
#else
class CctwTransformInterface
{
public:
  CctwTransformInterface();
#endif

#ifdef USE_QT
public slots:
#else
public:
#endif

  virtual bool hasInverse() const = 0;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D) = 0;
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q) = 0;
                                          // Back transform from output to input space coordinates
};

#endif // CCTWTRANSFORMINTERFACE_H
