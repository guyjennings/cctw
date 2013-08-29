#include "cctwtransformtest.h"
#include "cctwqtapplication.h"

CctwTransformTest::CctwTransformTest(CctwqtApplication *app, QObject *parent) :
  CctwqtObject(parent),
  m_Application(app),
  m_PixelSize(m_Application->saver(), this, "pixelSize", 0.080567, "Detector Pixel Size (mm)"),
  m_Wavelength(m_Application->saver(), this, "wavelength", 0.153870, "Wavelength (Angstrom)"),
  m_Distance(m_Application->saver(), this, "distance", 365.97, "Sample->Detector Distance (mm)"),
  m_GridOrigin(m_Application->saver(), this, "gridOrigin", QVector3D(1.5, -0.5, 0.0), "Grid Origin (recip lattice units)"),
  m_GridBasis(m_Application->saver(), this, "gridBasis",
              1.0, 0.0, 0.0,
              0.0, 0.5, 0.5,
              0.0, 0.5, -0.5, "Grid Basis (recip latt units)"),
  m_GridBasisInv(m_Application->saver(), this, "gridBasisInv", get_GridBasis().inverted(), "Grid Basis Inverse")
{
}
