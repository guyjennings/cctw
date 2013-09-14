#define _USE_MATH_DEFINES
#include <cmath>
#include "cctwqtsetuptransformdialog.h"
#include "ui_cctwqtsetuptransformdialog.h"
#include "cctwqtmainwindow.h"
#include "cctwqtapplication.h"

CctwqtSetupTransformDialog::CctwqtSetupTransformDialog(CctwqtApplication *app, CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupTransformDialog),
  m_Application(app)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  connect(ui->m_TwoThetaCorrection, SIGNAL(valueChanged(double)), this, SLOT(updateTwoTheta()));
  connect(ui->m_TwoThetaNom, SIGNAL(valueChanged(double)), this, SLOT(updateTwoTheta()));

  connect(ui->m_OmegaCorrection, SIGNAL(valueChanged(double)), this, SLOT(updateOmega()));
  connect(ui->m_OmegaStep, SIGNAL(valueChanged(double)), this, SLOT(updateOmega()));

  connect(ui->m_ChiCorrection, SIGNAL(valueChanged(double)), this, SLOT(updateChi()));
  connect(ui->m_ChiNom, SIGNAL(valueChanged(double)), this, SLOT(updateChi()));

  connect(ui->m_PhiCorrection, SIGNAL(valueChanged(double)), this, SLOT(updatePhi()));
  connect(ui->m_PhiNom, SIGNAL(valueChanged(double)), this, SLOT(updatePhi()));

  copyParametersToDialog();
}

CctwqtSetupTransformDialog::~CctwqtSetupTransformDialog()
{
  delete ui;
}

void CctwqtSetupTransformDialog::accept()
{
  copyDialogToParameters();

  QDialog::accept();
}

void CctwqtSetupTransformDialog::copyParametersToDialog()
{
  CctwqtCrystalCoordinateParameters *parms = m_Application->m_Parameters;
  CctwqtInputData *input = m_Application->m_InputData;

  ui->m_PixelDimensionX->setValue(input->dimensions().x());
  ui->m_PixelDimensionY->setValue(input->dimensions().y());

  ui->m_PixelSize->setValue(parms->get_PixelSize());
  ui->m_Wavelength->setValue(parms->get_Wavelength());
  ui->m_Distance->setValue(parms->get_Distance());

  CctwUnitCell cell = parms->get_UnitCell();

  ui->m_UnitCellA->setValue(cell.a());
  ui->m_UnitCellB->setValue(cell.b());
  ui->m_UnitCellC->setValue(cell.c());
  ui->m_UnitCellAlpha->setValue(rad2deg(cell.alpha()));
  ui->m_UnitCellBeta->setValue(rad2deg(cell.beta()));
  ui->m_UnitCellGamma->setValue(rad2deg(cell.gamma()));

  CctwDoubleMatrix3x3 ubMat = parms->get_UBMat();

  ui->m_UBMat00->setValue(ubMat(0,0));
  ui->m_UBMat01->setValue(ubMat(0,1));
  ui->m_UBMat02->setValue(ubMat(0,2));
  ui->m_UBMat10->setValue(ubMat(1,0));
  ui->m_UBMat11->setValue(ubMat(1,1));
  ui->m_UBMat12->setValue(ubMat(1,2));
  ui->m_UBMat20->setValue(ubMat(2,0));
  ui->m_UBMat21->setValue(ubMat(2,1));
  ui->m_UBMat22->setValue(ubMat(2,2));

  ui->m_Det0x->setValue(parms->get_Det0x());
  ui->m_Det0y->setValue(parms->get_Det0y());

  CctwDoubleVector3D xt = parms->get_XTrans();

  ui->m_XTransX->setValue(xt.x());
  ui->m_XTransY->setValue(xt.y());
  ui->m_XTransZ->setValue(xt.z());

  ui->m_OrientErrorDetPitch->setValue(rad2deg(parms->get_OrientErrorDetPitch()));
  ui->m_OrientErrorDetRoll->setValue(rad2deg(parms->get_OrientErrorDetRoll()));
  ui->m_OrientErrorDetYaw->setValue(rad2deg(parms->get_OrientErrorDetYaw()));
  ui->m_OrientErrorGonPitch->setValue(rad2deg(parms->get_OrientErrorGonPitch()));

  ui->m_TwoThetaCorrection->setValue(rad2deg(parms->get_TwoThetaCorrection()));
  ui->m_TwoThetaNom->setValue(rad2deg(parms->get_TwoThetaNom()));

  ui->m_OmegaCorrection->setValue(rad2deg(parms->get_OmegaCorrection()));
  ui->m_OmegaStep->setValue(rad2deg(parms->get_OmegaStep()));

  ui->m_ChiCorrection->setValue(rad2deg(parms->get_ChiCorrection()));
  ui->m_ChiNom->setValue(rad2deg(parms->get_ChiNom()));

  ui->m_PhiCorrection->setValue(rad2deg(parms->get_PhiCorrection()));
  ui->m_PhiNom->setValue(rad2deg(parms->get_PhiNom()));

  CctwDoubleVector3D gridOrigin = parms->get_GridOrigin();

  ui->m_GridOriginX->setValue(gridOrigin.x());
  ui->m_GridOriginY->setValue(gridOrigin.y());
  ui->m_GridOriginZ->setValue(gridOrigin.z());

  CctwDoubleMatrix3x3 gridBasis = parms->get_GridBasis();

  ui->m_GridBasis00->setValue(gridBasis(0,0));
  ui->m_GridBasis01->setValue(gridBasis(0,1));
  ui->m_GridBasis02->setValue(gridBasis(0,2));
  ui->m_GridBasis10->setValue(gridBasis(1,0));
  ui->m_GridBasis11->setValue(gridBasis(1,1));
  ui->m_GridBasis12->setValue(gridBasis(1,2));
  ui->m_GridBasis20->setValue(gridBasis(2,0));
  ui->m_GridBasis21->setValue(gridBasis(2,1));
  ui->m_GridBasis22->setValue(gridBasis(2,2));

  CctwDoubleVector3D gridDim = parms->get_GridDim();

  ui->m_GridDimX->setValue(gridDim.x());
  ui->m_GridDimY->setValue(gridDim.y());
  ui->m_GridDimZ->setValue(gridDim.z());
}

void CctwqtSetupTransformDialog::updateTwoTheta()
{
  ui->m_TwoThetaAngle->setValue(ui->m_TwoThetaNom->value() + ui->m_TwoThetaCorrection->value());
}

void CctwqtSetupTransformDialog::updateOmega()
{
  ui->m_OmegaInitial->setValue(ui->m_OmegaCorrection->value());
}

void CctwqtSetupTransformDialog::updateChi()
{
  ui->m_ChiAngle->setValue(ui->m_ChiNom->value() + ui->m_ChiCorrection->value());
}

void CctwqtSetupTransformDialog::updatePhi()
{
  ui->m_PhiAngle->setValue(ui->m_PhiNom->value() + ui->m_PhiCorrection->value());
}

void CctwqtSetupTransformDialog::copyDialogToParameters()
{
  CctwqtCrystalCoordinateParameters *parms = m_Application->m_Parameters;

  parms->set_PixelSize(ui->m_PixelSize->value());
  parms->set_Wavelength(ui->m_Wavelength->value());
  parms->set_Distance(ui->m_Distance->value());

  CctwUnitCell cell;

  cell.a() = ui->m_UnitCellA->value();
  cell.b() = ui->m_UnitCellB->value();
  cell.c() = ui->m_UnitCellC->value();
  cell.alpha() = deg2rad(ui->m_UnitCellAlpha->value());
  cell.beta() = deg2rad(ui->m_UnitCellBeta->value());
  cell.gamma() = deg2rad(ui->m_UnitCellGamma->value());

  parms->set_UnitCell(cell);

  CctwDoubleMatrix3x3 ubMat;

  ubMat(0,0) = ui->m_UBMat00->value();
  ubMat(0,1) = ui->m_UBMat01->value();
  ubMat(0,2) = ui->m_UBMat02->value();
  ubMat(1,0) = ui->m_UBMat10->value();
  ubMat(1,1) = ui->m_UBMat11->value();
  ubMat(1,2) = ui->m_UBMat12->value();
  ubMat(2,0) = ui->m_UBMat20->value();
  ubMat(2,1) = ui->m_UBMat21->value();
  ubMat(2,2) = ui->m_UBMat22->value();

  parms->set_UBMat(ubMat);

  parms->set_Det0x(ui->m_Det0x->value());
  parms->set_Det0y(ui->m_Det0y->value());

  CctwDoubleVector3D xt;

  xt.x() = ui->m_XTransX->value();
  xt.y() = ui->m_XTransY->value();
  xt.z() = ui->m_XTransZ->value();

  parms->set_XTrans(xt);

  parms->set_OrientErrorDetPitch(deg2rad(ui->m_OrientErrorDetPitch->value()));
  parms->set_OrientErrorDetRoll(deg2rad(ui->m_OrientErrorDetRoll->value()));
  parms->set_OrientErrorDetYaw(deg2rad(ui->m_OrientErrorDetYaw->value()));
  parms->set_OrientErrorGonPitch(deg2rad(ui->m_OrientErrorGonPitch->value()));

  parms->set_TwoThetaCorrection(deg2rad(ui->m_TwoThetaCorrection->value()));
  parms->set_TwoThetaNom(deg2rad(ui->m_TwoThetaNom->value()));

  parms->set_OmegaCorrection(deg2rad(ui->m_OmegaCorrection->value()));
  parms->set_OmegaStep(deg2rad(ui->m_OmegaStep->value()));

  parms->set_ChiCorrection(deg2rad(ui->m_ChiCorrection->value()));
  parms->set_ChiNom(deg2rad(ui->m_ChiNom->value()));

  parms->set_PhiCorrection(deg2rad(ui->m_PhiCorrection->value()));
  parms->set_PhiNom(deg2rad(ui->m_PhiNom->value()));

  CctwDoubleVector3D gridOrigin;

  gridOrigin.x() = ui->m_GridOriginX->value();
  gridOrigin.y() = ui->m_GridOriginY->value();
  gridOrigin.z() = ui->m_GridOriginZ->value();

  parms->set_GridOrigin(gridOrigin);

  CctwDoubleMatrix3x3 gridBasis;

  gridBasis(0,0) = ui->m_GridBasis00->value();
  gridBasis(0,1) = ui->m_GridBasis01->value();
  gridBasis(0,2) = ui->m_GridBasis02->value();
  gridBasis(1,0) = ui->m_GridBasis10->value();
  gridBasis(1,1) = ui->m_GridBasis11->value();
  gridBasis(1,2) = ui->m_GridBasis12->value();
  gridBasis(2,0) = ui->m_GridBasis20->value();
  gridBasis(2,1) = ui->m_GridBasis21->value();
  gridBasis(2,2) = ui->m_GridBasis22->value();

  parms->set_GridBasis(gridBasis);

  CctwDoubleVector3D gridDim;

  gridDim.x() = ui->m_GridDimX->value();
  gridDim.y() = ui->m_GridDimY->value();
  gridDim.z() = ui->m_GridDimZ->value();

  parms->set_GridDim(gridDim);
}

double CctwqtSetupTransformDialog::rad2deg(double rad)
{
  return rad*180.0/M_PI;
}

double CctwqtSetupTransformDialog::deg2rad(double deg)
{
  return deg*M_PI/180.0;
}
