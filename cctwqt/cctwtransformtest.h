#ifndef CCTWTRANSFORMTEST_H
#define CCTWTRANSFORMTEST_H

#include "cctwqtobject.h"
#include "cctwqtdoublematrix3x3property.h"
#include "cctwqtdoublevector3dproperty.h"

class CctwqtApplication;

class CctwTransformTest : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwTransformTest(CctwqtApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
  
private:
  CctwqtApplication *m_Application;

  Q_PROPERTY(double pixelSize READ get_PixelSize WRITE set_PixelSize)
  QCEP_DOUBLE_PROPERTY(PixelSize)

  Q_PROPERTY(double wavelength READ get_Wavelength WRITE set_Wavelength)
  QCEP_DOUBLE_PROPERTY(Wavelength)

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance)
  QCEP_DOUBLE_PROPERTY(Distance)

//  Q_PROPERTY(CctwUnitCell unitCell READ get_UnitCell WRITE set_UnitCell)
//  CCTWQT_UNITCELL_PROPERTY(UnitCell)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin READ get_GridOrigin WRITE set_GridOrigin)
  CCTWQT_DOUBLEVECTOR3D_PROPERTY(GridOrigin)

  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis READ get_GridBasis WRITE set_GridBasis)
  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(GridBasis)

  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasisInv READ get_GridBasisInv WRITE set_GridBasisInv)
  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(GridBasisInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 umat READ get_Umat WRITE set_Umat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(Umat)

//  Q_PROPERTY(CctwDoubleMatrix3x3 umatInv READ get_UmatInv WRITE set_UmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(UmatInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 bmat READ get_Bmat WRITE set_Bmat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(Bmat)

//  Q_PROPERTY(CctwDoubleMatrix3x3 bmatInv READ get_BmatInv WRITE set_BmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(BmatInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 ubmat READ get_UBmat WRITE set_UBmat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(UBmat)

//  Q_PROPERTY(CctwDoubleMatrix3x3 ubmatInv READ get_UBmatInv WRITE set_UBmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(UBmatInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 gmat READ get_Gmat WRITE set_Gmat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(Gmat)

//  Q_PROPERTY(CctwDoubleMatrix3x3 gmatInv READ get_GmatInv WRITE set_GmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(GmatInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 dmat READ get_Dmat WRITE set_Dmat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(Dmat)

//  Q_PROPERTY(CctwDoubleMatrix3x3 dmatInv READ get_DmatInv WRITE set_DmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(DmatInv)

//  Q_PROPERTY(CctwDoubleMatrix3x3 omat READ get_Omat WRITE set_Omat)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(Omat)

//  Q_PROPERTY(QMatrix3x3 omatInv READ get_OmatInv WRITE set_OmatInv)
//  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(OmatInv)

};

#endif // CCTWTRANSFORMTEST_H
