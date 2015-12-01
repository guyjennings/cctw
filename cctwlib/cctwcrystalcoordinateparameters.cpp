#include "cctwcrystalcoordinateparameters.h"
#include "cctwdebug.h"
#include "cctwapplication.h"

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#include <QRegExp>

CctwCrystalCoordinateParameters::CctwCrystalCoordinateParameters(QString name, QObject *parent) :
  CctwObject(name, parent),
  m_TwoTheta(QcepSettingsSaverWPtr(), this, "twoTheta", "", "twoTheta spec"),
  m_TwoThetaCorrection(QcepSettingsSaverWPtr(), this, "twoThetaCorrection", 0.0, "twoTheta correction"),
  m_TwoThetaNom(QcepSettingsSaverWPtr(), this, "twoThetaNom", 0.0, "twoTheta nominal (start)"),
  m_TwoThetaStep(QcepSettingsSaverWPtr(), this, "twoThetaStep", 0.0, "twoTheta step"),
  m_TwoThetaAngles(QcepSettingsSaverWPtr(), this, "twoThetaAngles", QcepDoubleVector(), "twoTheta Angles"),

  m_Omega(QcepSettingsSaverWPtr(), this, "omega", "", "omega spec"),
  m_OmegaCorrection(QcepSettingsSaverWPtr(), this, "omegaCorrection", 0.0, "omega correction"),
  m_OmegaNom(QcepSettingsSaverWPtr(), this, "omegaNom", 0.0, "omega nominal (start)"),
  m_OmegaStep(QcepSettingsSaverWPtr(), this, "omegaStep", 0.0, "omega step"),
  m_OmegaAngles(QcepSettingsSaverWPtr(), this, "omegaAngles", QcepDoubleVector(), "omega Angles"),

  m_Chi(QcepSettingsSaverWPtr(), this, "chi", "", "chi spec"),
  m_ChiCorrection(QcepSettingsSaverWPtr(), this, "chiCorrection", 0.0, "chi correction"),
  m_ChiNom(QcepSettingsSaverWPtr(), this, "chiNom", 0.0, "chi nominal (start)"),
  m_ChiStep(QcepSettingsSaverWPtr(), this, "chiStep", 0.0, "chi step"),
  m_ChiAngles(QcepSettingsSaverWPtr(), this, "chiAngles", QcepDoubleVector(), "chi Angles"),

  m_Phi(QcepSettingsSaverWPtr(), this, "phi", "", "phi spec"),
  m_PhiCorrection(QcepSettingsSaverWPtr(), this, "phiCorrection", 0.0, "phi correction"),
  m_PhiNom(QcepSettingsSaverWPtr(), this, "phiNom", 0.0, "phi nominal (start)"),
  m_PhiStep(QcepSettingsSaverWPtr(), this, "phiStep", 0.0, "phi step"),
  m_PhiAngles(QcepSettingsSaverWPtr(), this, "phiAngles", QcepDoubleVector(), "phi Angles"),

  m_ExtraFlip(QcepSettingsSaverWPtr(), this, "extraFlip", 1, "Extra Flip on input x->(2048-y), y->(2048-x)")
{
  setDefaults();
}

void CctwCrystalCoordinateParameters::setDefaults()
{
  setPixelSize(0.080567);
  setWavelength(0.153870);
  setDistance(365.97);

  setUnitCell(CctwUnitCell(4.04, 4.04, 4.04, 1.570796, 1.570796, 1.570796));

  setUBMat(CctwDoubleMatrix3x3(-0.001955, 0.177126, -0.172890,
                               -0.247516, -0.000885, 0.001892,
                               0.000736, 0.172899, 0.177127));

  setOMat(CctwDoubleMatrix3x3(0.0, -1.0, 0.0,
                              0.0, 0.0, 1.0,
                              -1.0, 0.0, 0.0));

  setDet0x(1041.266200);
  setDet0y(989.624800);

  setXTrans(CctwDoubleVector3D(0.005277, -0.006373, 0.034958));

  setOrientErrorDetPitch(-0.014305);
  setOrientErrorDetRoll(-0.008063);
  setOrientErrorDetYaw(-0.000361);
  setOrientErrorGonPitch(-0.000000);

  setGridOrigin(CctwDoubleVector3D(1.5, -0.5, 0.0));
  setGridBasis(CctwDoubleMatrix3x3(1.0, 0.0, 0.0,
                                   0.0, 0.5, 0.5,
                                   0.0, 0.5, -0.5));
  setGridDim(CctwDoubleVector3D(101,71,71));

  setGridOffset(CctwDoubleVector3D(1024,1024,1024));

  set_ExtraFlip(1);
}

double CctwCrystalCoordinateParameters::pixelSize() const { return m_PixelSize; }
double CctwCrystalCoordinateParameters::wavelength() const { return m_Wavelength; }
double CctwCrystalCoordinateParameters::distance() const { return m_Distance; }

CctwUnitCell        CctwCrystalCoordinateParameters::unitCell() const { return m_UnitCell; }
CctwDoubleMatrix3x3 CctwCrystalCoordinateParameters::ubMat() const { return m_UBMat; }
CctwDoubleMatrix3x3 CctwCrystalCoordinateParameters::oMat() const  { return m_OMat; }
CctwDoubleVector3D  CctwCrystalCoordinateParameters::oVec() const  { return m_OVec; }

double CctwCrystalCoordinateParameters::det0x() const { return m_Det0x; }
double CctwCrystalCoordinateParameters::det0y() const { return m_Det0y; }

CctwDoubleVector3D CctwCrystalCoordinateParameters::xTrans() const { return m_XTrans; }

double CctwCrystalCoordinateParameters::orientErrorDetPitch() const { return m_OrientErrorDetPitch; }
double CctwCrystalCoordinateParameters::orientErrorDetRoll() const { return m_OrientErrorDetRoll; }
double CctwCrystalCoordinateParameters::orientErrorDetYaw() const { return m_OrientErrorDetYaw; }
double CctwCrystalCoordinateParameters::orientErrorGonPitch() const { return m_OrientErrorGonPitch; }

CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridOrigin() const { return m_GridOrigin; }
CctwDoubleMatrix3x3 CctwCrystalCoordinateParameters::gridBasis() const { return m_GridBasis; }
CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridDim() const { return m_GridDim; }
CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridOffset() const { return m_GridOffset; }

void CctwCrystalCoordinateParameters::setPixelSize(double sz) { m_PixelSize = sz; }
void CctwCrystalCoordinateParameters::setWavelength(double wv) { m_Wavelength = wv; }
void CctwCrystalCoordinateParameters::setDistance(double d) { m_Distance = d; }

void CctwCrystalCoordinateParameters::setUnitCell(CctwUnitCell uc) { m_UnitCell = uc; }
void CctwCrystalCoordinateParameters::setUBMat(CctwDoubleMatrix3x3 ub) { m_UBMat = ub; }
void CctwCrystalCoordinateParameters::setOMat(CctwDoubleMatrix3x3 om) { m_OMat = om; }
void CctwCrystalCoordinateParameters::setOVec(CctwDoubleVector3D ov) { m_OVec = ov; }

void CctwCrystalCoordinateParameters::setDet0x(double d0x) { m_Det0x = d0x; }
void CctwCrystalCoordinateParameters::setDet0y(double d0y) { m_Det0y = d0y; }

void CctwCrystalCoordinateParameters::setXTrans(CctwDoubleVector3D xt) { m_XTrans = xt; }

void CctwCrystalCoordinateParameters::setOrientErrorDetPitch(double x) { m_OrientErrorDetPitch = x; }
void CctwCrystalCoordinateParameters::setOrientErrorDetRoll(double x) { m_OrientErrorDetRoll = x; }
void CctwCrystalCoordinateParameters::setOrientErrorDetYaw(double x) { m_OrientErrorDetYaw = x; }
void CctwCrystalCoordinateParameters::setOrientErrorGonPitch(double x) { m_OrientErrorGonPitch = x; }

void CctwCrystalCoordinateParameters::setTwoTheta(const QString tth) {
  set_TwoTheta(tth);

  parseAngleArgument(tth,"twoTheta", &m_TwoThetaCorrection, &m_TwoThetaNom, &m_TwoThetaStep, &m_TwoThetaAngles);
}

double CctwCrystalCoordinateParameters::twoThetaAngle(double frame)
{
  return interpolateAngle(frame, &m_TwoThetaCorrection, &m_TwoThetaNom, &m_TwoThetaStep, &m_TwoThetaAngles);
}

void CctwCrystalCoordinateParameters::setOmega(const QString omg) {
  set_Omega(omg);

  parseAngleArgument(omg, "omega", &m_OmegaCorrection, &m_OmegaNom, &m_OmegaStep, &m_OmegaAngles);
}

double CctwCrystalCoordinateParameters::omegaAngle(double frame)
{
  return interpolateAngle(frame, &m_OmegaCorrection, &m_OmegaNom, &m_OmegaStep, &m_OmegaAngles);
}

void CctwCrystalCoordinateParameters::setChi(const QString chi) {
  set_Chi(chi);

  parseAngleArgument(chi, "chi", &m_ChiCorrection, &m_ChiNom, &m_ChiStep, &m_ChiAngles);
}

double CctwCrystalCoordinateParameters::chiAngle(double frame)
{
  return interpolateAngle(frame, &m_ChiCorrection, &m_ChiNom, &m_ChiStep, prop_ChiAngles());
}

void CctwCrystalCoordinateParameters::setPhi(const QString phi) {
  set_Phi(phi);

  parseAngleArgument(phi, "phi", &m_PhiCorrection, &m_PhiNom, &m_PhiStep, &m_PhiAngles);
}

double CctwCrystalCoordinateParameters::phiAngle(double frame)
{
  return interpolateAngle(frame, &m_PhiCorrection, &m_PhiNom, &m_PhiStep, &m_PhiAngles);
}

void CctwCrystalCoordinateParameters::setGridOrigin(CctwDoubleVector3D org) { m_GridOrigin = org; }
void CctwCrystalCoordinateParameters::setGridBasis(CctwDoubleMatrix3x3 bas) { m_GridBasis = bas; }
void CctwCrystalCoordinateParameters::setGridDim(CctwDoubleVector3D dim) { m_GridDim = dim; }
void CctwCrystalCoordinateParameters::setGridOffset(CctwDoubleVector3D off) { m_GridOffset = off; }

//void CctwCrystalCoordinateParameters::setExtraFlip(int extra) { m_ExtraFlip = extra; }

void CctwCrystalCoordinateParameters::parseAngleArgument(
    const QString arg,
    const QString angleName,
    QcepDoubleProperty *corr,
    QcepDoubleProperty *start,
    QcepDoubleProperty *step,
    QcepDoubleVectorProperty *angs)
{
  QUrl url(arg);
  QString fileName    = "";
  QString dataSetName = "";
  QcepDoubleVector a;

  if (arg.length()) {

    if (qcepDebug(DEBUG_APP)) {
      printMessage(tr("%1.parseAngleArgument(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(arg)));

      printMessage(tr("scheme:   %1").arg(url.scheme()));
      printMessage(tr("host:     %1").arg(url.host()));
      printMessage(tr("path:     %1").arg(url.path()));
      printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
      printMessage(tr("query:    %1").arg(url.query()));
#endif
      printMessage(tr("fragment: %1").arg(url.fragment()));
    }

#if QT_VERSION >= 0x050000
    if (url.hasQuery()) {
      QUrlQuery qry(url);

      QList <QPair <QString, QString> > l = qry.queryItems();

      QPair<QString, QString> v;
      foreach (v, l) {
        if (qcepDebug(DEBUG_APP)) {
          printMessage(tr(" key:     %1").arg(v.first));
          printMessage(tr(" val:     %1").arg(v.second));
        }
      }

      if (qry.hasQueryItem("start") && start) {
        QString chunkSize = qry.queryItemValue("start");

        start->setValue(chunkSize.toDouble());
        printMessage(tr("%1 start at %2").arg(angleName).arg(start->value()));
      }

      if (qry.hasQueryItem("nom") && start) {
        QString s = qry.queryItemValue("nom");

        start->setValue(s.toDouble());
        printMessage(tr("%1 start At %2").arg(angleName).arg(start->value()));
      }

      if (qry.hasQueryItem("step") && step) {
        QString s      = qry.queryItemValue("step");

        step->setValue(s.toDouble());
        printMessage(tr("%1 step by %2").arg(angleName).arg(step->value()));
      }
    }
  }
#endif

  QRegExp n1("[-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?");
  QRegExp n2("([-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?),([-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?)");

  if (n1.exactMatch(arg)) { // One number...
    if (start) {
      start->setValue(arg.toDouble());
      printMessage(tr("%1 angle = %2").arg(angleName).arg(start->value()));
    }
  } else if (n2.exactMatch(arg)) { // Two numbers...
    if (qcepDebug(DEBUG_APP)) {
      printMessage(tr("Matched two numbers : count %1").arg(n2.captureCount()));
    }
    if (start && n2.captureCount()==2) {
      start->setValue(n2.cap(1).toDouble());
      printMessage(tr("%1 start at %2").arg(angleName).arg(start->value()));
    }
    if (step && n2.captureCount()==2) {
      step->setValue(n2.cap(2).toDouble());
      printMessage(tr("%1 step by %2").arg(angleName).arg(step->value()));
    }
  } else if (url.path().length() >= 1){
    fileName    = url.path();
    dataSetName = url.fragment();

    if (fileName.length() == 0) {
      printMessage(tr("No filename given for %1 angles, skipping").arg(angleName));
    }

    if (dataSetName.length() == 0) {
      printMessage(tr("No dataset name given for %1 angles, skipping").arg(angleName));
    }

    QFileInfo f(fileName);

    hid_t fileId = -1;
    hid_t dsetId = -1;
    hid_t dspcId = -1;

    try
    {
      if (!f.exists())
        throw tr("%1 angles file %2 does not exist").arg(angleName).arg(fileName);

      if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
        throw tr("%1 angles file %2 exists but is not an hdf file").arg(angleName).arg(fileName);

      fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDONLY, H5P_DEFAULT);

      if (fileId < 0)
        throw tr("%1 angles file %2 could not be opened").arg(angleName).arg(fileName);

      dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);

      if (dsetId <= 0)
        throw tr("Could not open %1 angles dataset %2").arg(angleName).arg(dataSetName);

      dspcId = H5Dget_space(dsetId);

      if (dspcId < 0)
        throw tr("Could not get dataspace of existing %1 angles dataset %2").arg(angleName).arg(dataSetName);

      hsize_t dims[3];
      int ndims = H5Sget_simple_extent_ndims(dspcId);

      if (ndims != 1)
        throw tr("%1 angles dataset %2, dataspace is not 1-dimensional").arg(angleName).arg(dataSetName);

      int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);

      if (ndims2 != 1)
        throw tr("Could not get %1 angles dataset %2 dimensions").arg(angleName).arg(dataSetName);

      if (angs == NULL)
        throw tr("Angles argument not given");

      a.resize(dims[0]);

      hid_t memspace_id = -1;
      hsize_t offset[1], count[1], stride[1], block[1];
      offset[0] = 0;
      count[0]  = a.size();
      stride[0] = 1;
      block[0]  = 1;

      double *angles = a.data();

      memspace_id = H5Screate_simple(1, count, NULL);

      herr_t selerr = H5Sselect_hyperslab(dspcId, H5S_SELECT_SET, offset, stride, count, block);

      if (selerr < 0)
        throw tr("Error selecting hyperslab for %1 failed, selerr = %2").arg(angleName).arg(selerr);

      herr_t rderr = H5Dread(dsetId,
                             H5T_NATIVE_DOUBLE,
                             memspace_id,
                             dspcId, H5P_DEFAULT, angles);

      if (rderr < 0)
        throw tr("Error reading %1 angles, rderr = %2").arg(angleName).arg(rderr);

      if (angs) {
        angs->setValue(a);
      }
    }

    catch (QString &msg)
    {
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (fileId >= 0) H5Fclose(fileId);
  }
}

double CctwCrystalCoordinateParameters::interpolateAngle(double frame,
    QcepDoubleProperty *corr,
    QcepDoubleProperty *start,
    QcepDoubleProperty *step,
    QcepDoubleVectorProperty *angs)
{
  double res;

  QcepDoubleVector a;

  if (angs) a=angs->value();

  if (a.length() == 0) {
    res = start->value() + frame*step->value() + corr->value();
  } else {
    int f0 = ::floor(frame);
    int f1 = f0+1;
    double df = frame-f0;

    res = corr->value() + a.value(f0)+df*(a.value(f1) - a.value(f0));
  }

  return res;
}
