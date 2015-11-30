#define _USE_MATH_DEFINES
#include <cmath>
#include "cctwqtsetuptransformdialog.h"
#include "ui_cctwqtsetuptransformdialog.h"
#include "cctwqtmainwindow.h"
#include "cctwapplication.h"

CctwqtSetupTransformDialog::CctwqtSetupTransformDialog(CctwApplication *app, CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupTransformDialog),
  m_Application(app)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

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
  CctwCrystalCoordinateParameters *parms = m_Application->m_Parameters;
  CctwChunkedData *input = m_Application->m_InputData;

  ui->m_PixelDimensionX->setValue(input->dimensions().x());
  ui->m_PixelDimensionY->setValue(input->dimensions().y());

  ui->m_PixelSize->setValue(parms->pixelSize());
  ui->m_Wavelength->setValue(parms->wavelength());
  ui->m_Distance->setValue(parms->distance());

  CctwUnitCell cell = parms->unitCell();

  ui->m_UnitCellA->setValue(cell.a());
  ui->m_UnitCellB->setValue(cell.b());
  ui->m_UnitCellC->setValue(cell.c());
  ui->m_UnitCellAlpha->setValue(rad2deg(cell.alpha()));
  ui->m_UnitCellBeta->setValue(rad2deg(cell.beta()));
  ui->m_UnitCellGamma->setValue(rad2deg(cell.gamma()));

  CctwDoubleMatrix3x3 ubMat = parms->ubMat();

  ui->m_UBMat00->setValue(ubMat(0,0));
  ui->m_UBMat01->setValue(ubMat(0,1));
  ui->m_UBMat02->setValue(ubMat(0,2));
  ui->m_UBMat10->setValue(ubMat(1,0));
  ui->m_UBMat11->setValue(ubMat(1,1));
  ui->m_UBMat12->setValue(ubMat(1,2));
  ui->m_UBMat20->setValue(ubMat(2,0));
  ui->m_UBMat21->setValue(ubMat(2,1));
  ui->m_UBMat22->setValue(ubMat(2,2));

  CctwDoubleMatrix3x3 oMat = parms->oMat();

  ui->m_OMat00->setValue(oMat(0,0));
  ui->m_OMat01->setValue(oMat(0,1));
  ui->m_OMat02->setValue(oMat(0,2));
  ui->m_OMat10->setValue(oMat(1,0));
  ui->m_OMat11->setValue(oMat(1,1));
  ui->m_OMat12->setValue(oMat(1,2));
  ui->m_OMat20->setValue(oMat(2,0));
  ui->m_OMat21->setValue(oMat(2,1));
  ui->m_OMat22->setValue(oMat(2,2));

  CctwDoubleVector3D oVec = parms->oVec();

  ui->m_OVec0->setValue(oVec(0));
  ui->m_OVec1->setValue(oVec(1));
  ui->m_OVec2->setValue(oVec(2));

  ui->m_Det0x->setValue(parms->det0x());
  ui->m_Det0y->setValue(parms->det0y());

  CctwDoubleVector3D xt = parms->xTrans();

  ui->m_XTransX->setValue(xt.x());
  ui->m_XTransY->setValue(xt.y());
  ui->m_XTransZ->setValue(xt.z());

  ui->m_OrientErrorDetPitch->setValue(rad2deg(parms->orientErrorDetPitch()));
  ui->m_OrientErrorDetRoll->setValue(rad2deg(parms->orientErrorDetRoll()));
  ui->m_OrientErrorDetYaw->setValue(rad2deg(parms->orientErrorDetYaw()));
  ui->m_OrientErrorGonPitch->setValue(rad2deg(parms->orientErrorGonPitch()));

  ui->m_TwoTheta->setText(parms->twoTheta());
  ui->m_TwoThetaCorrection->setValue(rad2deg(parms->twoThetaCorrection()));
  ui->m_TwoThetaNom->setValue(rad2deg(parms->twoThetaNom()));
  ui->m_TwoThetaStep->setValue(rad2deg(parms->twoThetaStep()));

  ui->m_Omega->setText(parms->omega());
  ui->m_OmegaCorrection->setValue(rad2deg(parms->omegaCorrection()));
  ui->m_OmegaNom->setValue(rad2deg(parms->omegaNom()));
  ui->m_OmegaStep->setValue(rad2deg(parms->omegaStep()));

  ui->m_Chi->setText(parms->chi());
  ui->m_ChiCorrection->setValue(rad2deg(parms->chiCorrection()));
  ui->m_ChiNom->setValue(rad2deg(parms->chiNom()));
  ui->m_ChiStep->setValue(rad2deg(parms->chiStep()));

  ui->m_Phi->setText(parms->phi());
  ui->m_PhiCorrection->setValue(rad2deg(parms->phiCorrection()));
  ui->m_PhiNom->setValue(rad2deg(parms->phiNom()));
  ui->m_PhiStep->setValue(rad2deg(parms->phiStep()));

  CctwDoubleVector3D gridOrigin = parms->gridOrigin();

  ui->m_GridOriginX->setValue(gridOrigin.x());
  ui->m_GridOriginY->setValue(gridOrigin.y());
  ui->m_GridOriginZ->setValue(gridOrigin.z());

  CctwDoubleMatrix3x3 gridBasis = parms->gridBasis();

  ui->m_GridBasis00->setValue(gridBasis(0,0));
  ui->m_GridBasis01->setValue(gridBasis(0,1));
  ui->m_GridBasis02->setValue(gridBasis(0,2));
  ui->m_GridBasis10->setValue(gridBasis(1,0));
  ui->m_GridBasis11->setValue(gridBasis(1,1));
  ui->m_GridBasis12->setValue(gridBasis(1,2));
  ui->m_GridBasis20->setValue(gridBasis(2,0));
  ui->m_GridBasis21->setValue(gridBasis(2,1));
  ui->m_GridBasis22->setValue(gridBasis(2,2));

  CctwDoubleVector3D gridDim = parms->gridDim();

  ui->m_GridDimX->setValue(gridDim.x());
  ui->m_GridDimY->setValue(gridDim.y());
  ui->m_GridDimZ->setValue(gridDim.z());

  CctwDoubleVector3D gridOffset = parms->gridOffset();

  ui->m_GridOffsetX->setValue(gridOffset.x());
  ui->m_GridOffsetY->setValue(gridOffset.y());
  ui->m_GridOffsetZ->setValue(gridOffset.z());
}

void CctwqtSetupTransformDialog::copyDialogToParameters()
{
  CctwCrystalCoordinateParameters *parms = m_Application->m_Parameters;

  parms->setPixelSize(ui->m_PixelSize->value());
  parms->setWavelength(ui->m_Wavelength->value());
  parms->setDistance(ui->m_Distance->value());

  CctwUnitCell cell;

  cell.a() = ui->m_UnitCellA->value();
  cell.b() = ui->m_UnitCellB->value();
  cell.c() = ui->m_UnitCellC->value();
  cell.alpha() = deg2rad(ui->m_UnitCellAlpha->value());
  cell.beta() = deg2rad(ui->m_UnitCellBeta->value());
  cell.gamma() = deg2rad(ui->m_UnitCellGamma->value());

  parms->setUnitCell(cell);

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

  parms->setUBMat(ubMat);

  CctwDoubleMatrix3x3 oMat;

  oMat(0,0) = ui->m_OMat00->value();
  oMat(0,1) = ui->m_OMat01->value();
  oMat(0,2) = ui->m_OMat02->value();
  oMat(1,0) = ui->m_OMat10->value();
  oMat(1,1) = ui->m_OMat11->value();
  oMat(1,2) = ui->m_OMat12->value();
  oMat(2,0) = ui->m_OMat20->value();
  oMat(2,1) = ui->m_OMat21->value();
  oMat(2,2) = ui->m_OMat22->value();

  parms->setOMat(oMat);

  CctwDoubleVector3D oVec;

  oVec(0) = ui->m_OVec0->value();
  oVec(1) = ui->m_OVec1->value();
  oVec(2) = ui->m_OVec2->value();

  parms->setOVec(oVec);

  parms->setDet0x(ui->m_Det0x->value());
  parms->setDet0y(ui->m_Det0y->value());

  CctwDoubleVector3D xt;

  xt.x() = ui->m_XTransX->value();
  xt.y() = ui->m_XTransY->value();
  xt.z() = ui->m_XTransZ->value();

  parms->setXTrans(xt);

  parms->setOrientErrorDetPitch(deg2rad(ui->m_OrientErrorDetPitch->value()));
  parms->setOrientErrorDetRoll(deg2rad(ui->m_OrientErrorDetRoll->value()));
  parms->setOrientErrorDetYaw(deg2rad(ui->m_OrientErrorDetYaw->value()));
  parms->setOrientErrorGonPitch(deg2rad(ui->m_OrientErrorGonPitch->value()));

  parms->setTwoThetaCorrection(deg2rad(ui->m_TwoThetaCorrection->value()));
  parms->setTwoThetaNom(deg2rad(ui->m_TwoThetaNom->value()));
  parms->setTwoThetaStep(deg2rad(ui->m_TwoThetaStep->value()));
  parms->setTwoTheta(ui->m_TwoTheta->text());

  parms->setOmegaCorrection(deg2rad(ui->m_OmegaCorrection->value()));
  parms->setOmegaNom(deg2rad(ui->m_OmegaNom->value()));
  parms->setOmegaStep(deg2rad(ui->m_OmegaStep->value()));
  parms->setOmega(ui->m_Omega->text());

  parms->setChiCorrection(deg2rad(ui->m_ChiCorrection->value()));
  parms->setChiNom(deg2rad(ui->m_ChiNom->value()));
  parms->setChiStep(deg2rad(ui->m_ChiStep->value()));
  parms->setChi(ui->m_Chi->text());

  parms->setPhiCorrection(deg2rad(ui->m_PhiCorrection->value()));
  parms->setPhiNom(deg2rad(ui->m_PhiNom->value()));
  parms->setPhiStep(deg2rad(ui->m_PhiStep->value()));
  parms->setPhi(ui->m_Phi->text());

  CctwDoubleVector3D gridOrigin;

  gridOrigin.x() = ui->m_GridOriginX->value();
  gridOrigin.y() = ui->m_GridOriginY->value();
  gridOrigin.z() = ui->m_GridOriginZ->value();

  parms->setGridOrigin(gridOrigin);

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

  parms->setGridBasis(gridBasis);

  CctwDoubleVector3D gridDim;

  gridDim.x() = ui->m_GridDimX->value();
  gridDim.y() = ui->m_GridDimY->value();
  gridDim.z() = ui->m_GridDimZ->value();

  parms->setGridDim(gridDim);

  CctwDoubleVector3D gridOffset;

  gridOffset.x() = ui->m_GridOffsetX->value();
  gridOffset.y() = ui->m_GridOffsetY->value();
  gridOffset.z() = ui->m_GridOffsetZ->value();

  parms->setGridOffset(gridOffset);
}

double CctwqtSetupTransformDialog::rad2deg(double rad)
{
  return rad*180.0/M_PI;
}

double CctwqtSetupTransformDialog::deg2rad(double deg)
{
  return deg*M_PI/180.0;
}
