#include "cctwqtcrystalcoordinateparameters.h"
#include "cctwqtapplication.h"

CctwqtCrystalCoordinateParameters::CctwqtCrystalCoordinateParameters(CctwqtApplication *app, QObject *parent) :
  CctwqtObject(parent),
  CctwCrystalCoordinateParameters(),
  m_Application(app),
  m_PixelSize(m_Application->saver(), this, "pixelSize", 0.080567, "Detector Pixel Size (mm)"),
  m_Wavelength(m_Application->saver(), this, "wavelength", 0.153870, "Wavelength (Angstrom)"),
  m_Distance(m_Application->saver(), this, "distance", 365.97, "Sample->Detector Distance (mm)"),

  m_UnitCell(m_Application->saver(), this, "unitCell",
             4.04, 4.04, 4.04, 1.570796, 1.570796, 1.570796,
             "Sample Unit Cell (Angstrom,Rad)"),

  m_UBMat(m_Application->saver(), this, "ubMat",
          -0.001955, 0.177126, -0.172890,
          -0.247516, -0.000885, 0.001892,
          0.000736, 0.172899, 0.177127,
          "UB Matrix"),

  m_Det0x(m_Application->saver(), this, "det0x", 1041.266200, "X beam center (mm)"),
  m_Det0y(m_Application->saver(), this, "det0y", 989.624800, "Y beam center (mm)"),

  m_XTrans(m_Application->saver(), this, "xTrans",
           0.005277, -0.006373, 0.034958, "XTrans (mm) goniometer head frame"),

  m_OrientErrorDetPitch(m_Application->saver(), this, "orientErrorDetPitch",
                        -0.014305, "Det Pitch Orientation Error (rad)"),
  m_OrientErrorDetRoll(m_Application->saver(), this, "orientErrorDetRoll",
                        -0.008063, "Det Roll Orientation Error (rad)"),
  m_OrientErrorDetYaw(m_Application->saver(), this, "orientErrorDetYaw",
                        -0.000361, "Det Yaw Orientation Error (rad)"),
  m_OrientErrorGonPitch(m_Application->saver(), this, "orientErrorGonPitch",
                        -0.000000, "Gon Pitch Orientation Error (rad)"),

  m_TwoThetaCorrection(m_Application->saver(), this, "twoThetaCorrection", 0.0, "Two Theta Correction (rad)"),
  m_TwoThetaNom(m_Application->saver(), this, "twoThetaNom", 0.0, "Two Theta Nominal (rad)"),

  m_OmegaCorrection(m_Application->saver(), this, "omegaCorrection", 0.003777, "Omega Correction (rad)"),
  m_OmegaStep(m_Application->saver(), this, "omegaStep", 0.008727, "Omega Step (rad)"),

  m_ChiCorrection(m_Application->saver(), this, "chiCorrection", -0.005531, "Chi Correction (rad)"),
  m_ChiNom(m_Application->saver(), this, "chiNom", 0.0, "Chi Nominal (rad)"),

  m_PhiCorrection(m_Application->saver(), this, "phiCorrection", 0.0, "Phi Correction (rad)"),
  m_PhiNom(m_Application->saver(), this, "phiNom", 0.0, "Phi Nominal (rad)"),

  m_GridOrigin(m_Application->saver(), this, "gridOrigin",
               1.5, -0.5, 0.0, "Grid Origin (recip lattice units)"),
  m_GridBasis(m_Application->saver(), this, "gridBasis",
              1.0, 0.0, 0.0,
              0.0, 0.5, 0.5,
              0.0, 0.5, -0.5, "Grid Basis (recip latt units)"),
  m_GridDim(m_Application->saver(), this, "gridDim", CctwDoubleVector3D(101,71,71), "Grid Dimensions")
{
  connect(&m_PixelSize, SIGNAL(valueChanged(double,int)), this, SLOT(updateParameters()));
  connect(&m_Wavelength, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_Distance, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_UnitCell, SIGNAL(valueChanged(CctwUnitCell,int)), this,  SLOT(updateParameters()));
  connect(&m_UBMat, SIGNAL(valueChanged(CctwDoubleMatrix3x3,int)), this,  SLOT(updateParameters()));
  connect(&m_Det0x, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_Det0y, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_XTrans, SIGNAL(valueChanged(CctwDoubleVector3D,int)), this,  SLOT(updateParameters()));
  connect(&m_OrientErrorDetPitch, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_OrientErrorDetRoll, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_OrientErrorDetYaw, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_OrientErrorGonPitch, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));

  connect(&m_TwoThetaCorrection, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_TwoThetaNom, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));

  connect(&m_OmegaCorrection, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_OmegaStep, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));

  connect(&m_ChiCorrection, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_ChiNom, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));

  connect(&m_PhiCorrection, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));
  connect(&m_PhiNom, SIGNAL(valueChanged(double,int)), this,  SLOT(updateParameters()));

  connect(&m_GridOrigin, SIGNAL(valueChanged(CctwDoubleVector3D,int)), this,  SLOT(updateParameters()));
  connect(&m_GridBasis, SIGNAL(valueChanged(CctwDoubleMatrix3x3,int)), this,  SLOT(updateParameters()));
  connect(&m_GridDim, SIGNAL(valueChanged(CctwDoubleVector3D,int)), this,  SLOT(updateParameters()));

  updateParameters();
}

void CctwqtCrystalCoordinateParameters::updateParameters()
{
  inherited::m_PixelSize = get_PixelSize();
  inherited::m_Wavelength = get_Wavelength();
  inherited::m_Distance = get_Distance();
  inherited::m_UnitCell = get_UnitCell();
  inherited::m_UBMat = get_UBMat();
  inherited::m_Det0x = get_Det0x();
  inherited::m_Det0y = get_Det0y();
  inherited::m_XTrans = get_XTrans();
  inherited::m_OrientErrorDetPitch = get_OrientErrorDetPitch();
  inherited::m_OrientErrorDetRoll = get_OrientErrorDetRoll();
  inherited::m_OrientErrorDetYaw = get_OrientErrorDetYaw();
  inherited::m_OrientErrorGonPitch = get_OrientErrorGonPitch();

  inherited::m_TwoThetaCorrection = get_TwoThetaCorrection();
  inherited::m_TwoThetaNom = get_TwoThetaNom();

  inherited::m_OmegaCorrection = get_OmegaCorrection();
  inherited::m_OmegaStep = get_OmegaStep();

  inherited::m_ChiCorrection = get_ChiCorrection();
  inherited::m_ChiNom = get_ChiNom();

  inherited::m_PhiCorrection = get_PhiCorrection();
  inherited::m_PhiNom = get_PhiNom();

  inherited::m_GridOrigin = get_GridOrigin();
  inherited::m_GridBasis = get_GridBasis();
  inherited::m_GridDim = get_GridDim();
}
