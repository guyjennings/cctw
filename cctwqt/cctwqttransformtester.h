#ifndef CCTWQTTRANSFORMTESTER_H
#define CCTWQTTRANSFORMTESTER_H

#include "cctwobject.h"
#include "cctwvector3d.h"

class CctwqtMainWindow;
class CctwCrystalCoordinateParameters;

class CctwqtTransformTester : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwqtTransformTester(CctwqtMainWindow *w, CctwCrystalCoordinateParameters *p, QObject *parent);

signals:

public slots:
  void parametersChanged();

  void testGetDetPix();
  void testPixel2QLab();
  void testQLab2HKL();
  void testHKL2Grid();
  void testGrid2HKL();
  void testHKL2QLab();
  void testQLab2Pixel();
  void testGetDetPos();

private:
  double getRotation();
  CctwDoubleVector3D getDet();
  void setDet(CctwDoubleVector3D det);
  CctwDoubleVector3D getPixels();
  void setPixels(CctwDoubleVector3D val);
  CctwDoubleVector3D getQLab();
  void setQLab(CctwDoubleVector3D val);
  CctwDoubleVector3D getHKL();
  void setHKL(CctwDoubleVector3D val);
  CctwDoubleVector3D getGrid();
  void setGrid(CctwDoubleVector3D val);

  void setColor(QDoubleSpinBox *box, QColor col);
  void unhighlight();
  void highlightInput(QDoubleSpinBox *inp);
  void highlightOutput(QDoubleSpinBox *inp);

private:
  CctwqtMainWindow                  *m_MainWindow;
  CctwCrystalCoordinateParameters   *m_Parameters;
};

#endif // CCTWQTTRANSFORMTESTER_H
