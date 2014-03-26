#include "cctwqttransformtester.h"
#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"
#include "cctwcrystalcoordinateparameters.h"
#include "cctwcrystalcoordinatetransform.h"

CctwqtTransformTester::CctwqtTransformTester(CctwqtMainWindow *w, CctwCrystalCoordinateParameters *p, QObject *parent) :
  CctwObject("transformtester", parent),
  m_MainWindow(w),
  m_Parameters(p)
{
  printMessage(tr("Transform tester constructed, parameters = %1").arg((long) m_Parameters));

  connect(m_Parameters, SIGNAL(parametersChanged()), this, SLOT(parametersChanged()));

  connect(m_MainWindow->ui->m_GetDetPixButton,  SIGNAL(clicked()), this, SLOT(testGetDetPix()));
  connect(m_MainWindow->ui->m_Pixel2QLabButton, SIGNAL(clicked()), this, SLOT(testPixel2QLab()));
  connect(m_MainWindow->ui->m_QLab2HKLButton,   SIGNAL(clicked()), this, SLOT(testQLab2HKL()));
  connect(m_MainWindow->ui->m_HKL2GridButton,   SIGNAL(clicked()), this, SLOT(testHKL2Grid()));
  connect(m_MainWindow->ui->m_Grid2HKLButton,   SIGNAL(clicked()), this, SLOT(testGrid2HKL()));
  connect(m_MainWindow->ui->m_HKL2QLabButton,   SIGNAL(clicked()), this, SLOT(testHKL2QLab()));
  connect(m_MainWindow->ui->m_QLab2PixelButton, SIGNAL(clicked()), this, SLOT(testQLab2Pixel()));
  connect(m_MainWindow->ui->m_GetDetPosButton,  SIGNAL(clicked()), this, SLOT(testGetDetPos()));
}

void CctwqtTransformTester::parametersChanged()
{
  printMessage("CctwqtTransformTester::parametersChanged");
}

void CctwqtTransformTester::setColor(QDoubleSpinBox *box, QColor col)
{
//  printMessage(tr("setColor %1 to %2").arg(box->objectName()).arg(col.hue()));

  QPalette pall = box->palette();

  pall.setColor(QPalette::Text, col);

  box->setPalette(pall);
  box->update();
}

void CctwqtTransformTester::unhighlight()
{
  setColor(m_MainWindow->ui->m_FrameAngle, Qt::black);

  setColor(m_MainWindow->ui->m_TestDetectorX, Qt::black);
  setColor(m_MainWindow->ui->m_TestDetectorY, Qt::black);
  setColor(m_MainWindow->ui->m_TestDetectorZ, Qt::black);

  setColor(m_MainWindow->ui->m_TestPixelX, Qt::black);
  setColor(m_MainWindow->ui->m_TestPixelY, Qt::black);
  setColor(m_MainWindow->ui->m_TestPixelZ, Qt::black);

  setColor(m_MainWindow->ui->m_TestQLabX, Qt::black);
  setColor(m_MainWindow->ui->m_TestQLabY, Qt::black);
  setColor(m_MainWindow->ui->m_TestQLabZ, Qt::black);

  setColor(m_MainWindow->ui->m_TestHKLX, Qt::black);
  setColor(m_MainWindow->ui->m_TestHKLY, Qt::black);
  setColor(m_MainWindow->ui->m_TestHKLZ, Qt::black);

  setColor(m_MainWindow->ui->m_TestGridX, Qt::black);
  setColor(m_MainWindow->ui->m_TestGridY, Qt::black);
  setColor(m_MainWindow->ui->m_TestGridZ, Qt::black);
}

void CctwqtTransformTester::highlightInput(QDoubleSpinBox *box)
{
  setColor(box, QColor(0,160,0));
}

void CctwqtTransformTester::highlightOutput(QDoubleSpinBox *box)
{
  setColor(box, QColor(220,0,0));
}

double CctwqtTransformTester::getRotation()
{
  double rot = m_MainWindow->ui->m_FrameAngle->value();

  highlightInput(m_MainWindow->ui->m_FrameAngle);

  return rot;
}

CctwDoubleVector3D CctwqtTransformTester::getDet()
{
  CctwDoubleVector3D val(m_MainWindow->ui->m_TestDetectorX->value(),
                         m_MainWindow->ui->m_TestDetectorY->value(),
                         m_MainWindow->ui->m_TestDetectorZ->value());

  highlightInput(m_MainWindow->ui->m_TestDetectorX);
  highlightInput(m_MainWindow->ui->m_TestDetectorY);
  highlightInput(m_MainWindow->ui->m_TestDetectorZ);

  return val;
}

void CctwqtTransformTester::setDet(CctwDoubleVector3D det)
{
  m_MainWindow->ui->m_TestDetectorX->setValue(det.x());
  m_MainWindow->ui->m_TestDetectorY->setValue(det.y());
  m_MainWindow->ui->m_TestDetectorZ->setValue(det.z());

  highlightOutput(m_MainWindow->ui->m_TestDetectorX);
  highlightOutput(m_MainWindow->ui->m_TestDetectorY);
  highlightOutput(m_MainWindow->ui->m_TestDetectorZ);
}

CctwDoubleVector3D CctwqtTransformTester::getPixels()
{
  CctwDoubleVector3D val(m_MainWindow->ui->m_TestPixelX->value(),
                         m_MainWindow->ui->m_TestPixelY->value(),
                         m_MainWindow->ui->m_TestPixelZ->value());

  highlightInput(m_MainWindow->ui->m_TestPixelX);
  highlightInput(m_MainWindow->ui->m_TestPixelY);
  highlightInput(m_MainWindow->ui->m_TestPixelZ);

  return val;
}

void CctwqtTransformTester::setPixels(CctwDoubleVector3D val)
{
  m_MainWindow->ui->m_TestPixelX->setValue(val.x());
  m_MainWindow->ui->m_TestPixelY->setValue(val.y());
  m_MainWindow->ui->m_TestPixelZ->setValue(val.z());

  highlightOutput(m_MainWindow->ui->m_TestPixelX);
  highlightOutput(m_MainWindow->ui->m_TestPixelY);
  highlightOutput(m_MainWindow->ui->m_TestPixelZ);
}

CctwDoubleVector3D CctwqtTransformTester::getQLab()
{
  CctwDoubleVector3D val(m_MainWindow->ui->m_TestQLabX->value(),
                         m_MainWindow->ui->m_TestQLabY->value(),
                         m_MainWindow->ui->m_TestQLabZ->value());

  highlightInput(m_MainWindow->ui->m_TestQLabX);
  highlightInput(m_MainWindow->ui->m_TestQLabY);
  highlightInput(m_MainWindow->ui->m_TestQLabZ);

  return val;
}

void CctwqtTransformTester::setQLab(CctwDoubleVector3D val)
{
  m_MainWindow->ui->m_TestQLabX->setValue(val.x());
  m_MainWindow->ui->m_TestQLabY->setValue(val.y());
  m_MainWindow->ui->m_TestQLabZ->setValue(val.z());

  highlightOutput(m_MainWindow->ui->m_TestQLabX);
  highlightOutput(m_MainWindow->ui->m_TestQLabY);
  highlightOutput(m_MainWindow->ui->m_TestQLabZ);
}

CctwDoubleVector3D CctwqtTransformTester::getHKL()
{
  CctwDoubleVector3D val(m_MainWindow->ui->m_TestHKLX->value(),
                         m_MainWindow->ui->m_TestHKLY->value(),
                         m_MainWindow->ui->m_TestHKLZ->value());

  highlightInput(m_MainWindow->ui->m_TestHKLX);
  highlightInput(m_MainWindow->ui->m_TestHKLY);
  highlightInput(m_MainWindow->ui->m_TestHKLZ);

  return val;
}

void CctwqtTransformTester::setHKL(CctwDoubleVector3D val)
{
  m_MainWindow->ui->m_TestHKLX->setValue(val.x());
  m_MainWindow->ui->m_TestHKLY->setValue(val.y());
  m_MainWindow->ui->m_TestHKLZ->setValue(val.z());

  highlightOutput(m_MainWindow->ui->m_TestHKLX);
  highlightOutput(m_MainWindow->ui->m_TestHKLY);
  highlightOutput(m_MainWindow->ui->m_TestHKLZ);
}

CctwDoubleVector3D CctwqtTransformTester::getGrid()
{
  CctwDoubleVector3D val(m_MainWindow->ui->m_TestGridX->value(),
                         m_MainWindow->ui->m_TestGridY->value(),
                         m_MainWindow->ui->m_TestGridZ->value());

  highlightInput(m_MainWindow->ui->m_TestGridX);
  highlightInput(m_MainWindow->ui->m_TestGridY);
  highlightInput(m_MainWindow->ui->m_TestGridZ);

  return val;
}

void CctwqtTransformTester::setGrid(CctwDoubleVector3D val)
{
  m_MainWindow->ui->m_TestGridX->setValue(val.x());
  m_MainWindow->ui->m_TestGridY->setValue(val.y());
  m_MainWindow->ui->m_TestGridZ->setValue(val.z());

  highlightOutput(m_MainWindow->ui->m_TestGridX);
  highlightOutput(m_MainWindow->ui->m_TestGridY);
  highlightOutput(m_MainWindow->ui->m_TestGridZ);
}

void CctwqtTransformTester::testGetDetPos()
{
  unhighlight();

  CctwDoubleVector3D det = getDet();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setPixels(t.getDetPos(det));
}

void CctwqtTransformTester::testPixel2QLab()
{
  unhighlight();

  CctwDoubleVector3D pix = getPixels();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setQLab(t.pixel2qlab(pix));
}

void CctwqtTransformTester::testQLab2HKL()
{
  unhighlight();

  CctwDoubleVector3D qlab = getQLab();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setHKL(t.qlab2hkl(qlab));
}

void CctwqtTransformTester::testHKL2Grid()
{
  unhighlight();

  CctwDoubleVector3D hkl = getHKL();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setGrid(t.hkl2grid(hkl));
}

void CctwqtTransformTester::testGrid2HKL()
{
  unhighlight();

  CctwDoubleVector3D grid = getGrid();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setHKL(t.grid2hkl(grid));
}

void CctwqtTransformTester::testHKL2QLab()
{
  unhighlight();

  CctwDoubleVector3D hkl = getHKL();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setQLab(t.hkl2qlab(hkl));
}

void CctwqtTransformTester::testQLab2Pixel()
{
  unhighlight();

  CctwDoubleVector3D qlab = getQLab();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setPixels(t.qlab2pixel(qlab));
}

void CctwqtTransformTester::testGetDetPix()
{
  unhighlight();

  CctwDoubleVector3D pix = getPixels();

  CctwCrystalCoordinateTransform t(m_Parameters, "test", this);
  t.setCurrentFrame(getRotation());

  setDet(t.getDetPix(pix));
}
