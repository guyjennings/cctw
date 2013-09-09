#ifndef CCTWCRYSTALCOORDINATEPARAMETERS_H
#define CCTWCRYSTALCOORDINATEPARAMETERS_H

#include "cctwvector3d.h"
#include "cctwmatrix3x3.h"
#include "cctwunitcell.h"

class CctwCrystalCoordinateParameters
{
public:
  CctwCrystalCoordinateParameters();

  double m_PixelSize;
  double m_Wavelength;
  double m_Distance;

  CctwUnitCell        m_UnitCell;
  CctwDoubleMatrix3x3 m_UBMat;

  double m_Det0x;
  double m_Det0y;

  CctwDoubleVector3D m_XTrans;

  double m_OrientErrorDetPitch;
  double m_OrientErrorDetRoll;
  double m_OrientErrorDetYaw;
  double m_OrientErrorGonPitch;

  double m_TwoThetaCorrection;
  double m_TwoThetaNom;

  double m_OmegaCorrection;
  double m_OmegaStep;

  double m_ChiCorrection;
  double m_ChiNom;

  double m_PhiCorrection;
  double m_PhiNom;

  CctwDoubleVector3D  m_GridOrigin;
  CctwDoubleMatrix3x3 m_GridBasis;
  CctwDoubleVector3D  m_GridDim;
};

#endif // CCTWCRYSTALCOORDINATEPARAMETERS_H
