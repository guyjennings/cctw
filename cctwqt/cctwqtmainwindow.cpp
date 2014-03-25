#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"
#include <QLineEdit>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_zoomer.h"
#include "qwt_symbol.h"
#include "QtConcurrentRun"

CctwqtMainWindow::CctwqtMainWindow(CctwApplication *app, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CctwqtMainWindow),
  m_Application(app),
  m_SetupInputDialog(NULL),
  m_SetupOutputDialog(NULL),
  m_SetupTransformDialog(NULL),
  m_Legend(NULL),
  m_Panner(NULL),
  m_Magnifier(NULL),
  m_Zoomer(NULL)
{
  ui->setupUi(this);

  connect(ui->m_CommandInput, SIGNAL(returnPressed()), this, SLOT(doEvaluateCommand()));

  connect(ui->m_SetupImportButton, SIGNAL(clicked()), this, SLOT(doSetupImport()));
  connect(ui->m_ActionSetupDataImport, SIGNAL(triggered()), this, SLOT(doSetupImport()));

  connect(ui->m_ImportButton, SIGNAL(clicked()), this, SLOT(doImport()));
  connect(ui->m_ActionImportData, SIGNAL(triggered()), this, SLOT(doImport()));

  connect(ui->m_SetupInputButton, SIGNAL(clicked()), this, SLOT(doSetupInput()));
  connect(ui->m_SetupOutputButton, SIGNAL(clicked()), this, SLOT(doSetupOutput()));
  connect(ui->m_SetupTransformButton, SIGNAL(clicked()), this, SLOT(doSetupTransform()));
  connect(ui->m_TransformButton, SIGNAL(clicked()), this, SLOT(doTransform()));
  connect(ui->m_HaltButton, SIGNAL(clicked()), this, SLOT(doHalt()));
  connect(ui->m_DependenciesButton, SIGNAL(clicked()), m_Application, SLOT(calculateDependencies()));
  connect(ui->m_SaveDepsButton, SIGNAL(clicked()), this, SLOT(doSaveDependencies()));
  connect(ui->m_LoadDepsButton, SIGNAL(clicked()), this, SLOT(doLoadDependencies()));
  connect(ui->m_PEMetaDataButton, SIGNAL(clicked()), this, SLOT(doAnalyzePEMetaData()));
  connect(ui->m_AnalyzeSpecDataButton, SIGNAL(clicked()), this, SLOT(doAnalyzeSpecDataFile()));

  connect(ui->m_ActionDependencies, SIGNAL(triggered()), m_Application, SLOT(calculateDependencies()));
  connect(ui->m_ActionLoadDependencies, SIGNAL(triggered()), this, SLOT(doLoadDependencies()));
  connect(ui->m_ActionSaveDependencies, SIGNAL(triggered()), this, SLOT(doSaveDependencies()));
  connect(ui->m_ActionReportDependencies, SIGNAL(triggered()), m_Application, SLOT(reportDependencies()));
  connect(ui->m_ActionAnalyzePEMetaData, SIGNAL(triggered()), this, SLOT(doAnalyzePEMetaData()));
  connect(ui->m_ActionAnalyseSpecDataFile, SIGNAL(triggered()), this, SLOT(doAnalyzeSpecDataFile()));

  connect(ui->m_ActionCompareTwoHDF5, SIGNAL(triggered()), this, SLOT(doCompareHDF5()));
  connect(ui->m_ActionCheckImportedData, SIGNAL(triggered()), this, SLOT(doCheckImportedData()));

  connect(ui->m_ActionLoadSettings, SIGNAL(triggered()), this, SLOT(doLoadSettings()));
  connect(ui->m_ActionSaveSettings, SIGNAL(triggered()), this, SLOT(doSaveSettings()));
  connect(ui->m_ActionQuit, SIGNAL(triggered()), this, SLOT(possiblyClose()));

  app->prop_Halting()->linkTo(ui->m_Halting);

  CctwImporter *import = app->m_ImportData;

  if (import) {
    import->prop_DarkImagePath()->linkTo(ui->m_ImportDarkImagePath);
    import->prop_ImageDirectory()->linkTo(ui->m_ImportDataDirectory);
    import->prop_OutputPath()->linkTo(ui->m_ImportOutputPath);
    import->prop_OutputDataset()->linkTo(ui->m_ImportOutputDataset);
    import->prop_Compression()->linkTo(ui->m_ImportOutputCompression);

    connect(import->prop_ImagePaths(), SIGNAL(valueChanged(QStringList,int)), this, SLOT(updateImportImagePaths(QStringList)));
    connect(import->prop_ChunkSize(), SIGNAL(valueChanged(CctwIntVector3D,int)), this, SLOT(updateImportChunkSize(CctwIntVector3D)));

    updateImportImagePaths(import->get_ImagePaths());
    updateImportChunkSize(import->get_ChunkSize());
  }

  CctwChunkedData *inputData = app->m_InputData;

  if (inputData) {
    inputData->prop_DataFileName()->linkTo(ui->m_InputDataFileName);
    inputData->prop_DataSetName()->linkTo(ui->m_InputDataSetName);

    connect(inputData->prop_HDFChunkSize(), SIGNAL(valueChanged(CctwIntVector3D,int)), this, SLOT(updateInputHDF5ChunkSize(CctwIntVector3D)));
    connect(inputData, SIGNAL(chunkSizeChanged(CctwIntVector3D)), this, SLOT(updateInputDimensions()));
    connect(inputData, SIGNAL(dimensionsChanged(CctwIntVector3D)), this, SLOT(updateInputDimensions()));
    connect(inputData, SIGNAL(chunkCountChanged(CctwIntVector3D)), this, SLOT(updateInputDimensions()));

    inputData->prop_Compression()->linkTo(ui->m_InputCompression);

    updateInputHDF5ChunkSize(inputData->get_HDFChunkSize());

    updateInputDimensions();
  }

  CctwChunkedData *outputData = app->m_OutputData;

  if (outputData) {
    outputData->prop_DataFileName()->linkTo(ui->m_OutputDataFileName);
    outputData->prop_DataSetName()->linkTo(ui->m_OutputDataSetName);

    connect(outputData->prop_HDFChunkSize(), SIGNAL(valueChanged(CctwIntVector3D,int)), this, SLOT(updateOutputHDF5ChunkSize(CctwIntVector3D)));
    connect(outputData, SIGNAL(chunkSizeChanged(CctwIntVector3D)), this, SLOT(updateOutputDimensions()));
    connect(outputData, SIGNAL(dimensionsChanged(CctwIntVector3D)), this, SLOT(updateOutputDimensions()));
    connect(outputData, SIGNAL(chunkCountChanged(CctwIntVector3D)), this, SLOT(updateOutputDimensions()));

    outputData->prop_Compression()->linkTo(ui->m_OutputCompression);

    updateOutputHDF5ChunkSize(outputData->get_HDFChunkSize());

    updateOutputDimensions();
  }

  connect(app->prop_Progress(), SIGNAL(valueChanged(int,int)), this, SLOT(onProgressUpdate()));
  connect(app->prop_ProgressLimit(), SIGNAL(valueChanged(int,int)), this, SLOT(onProgressUpdate()));

  qRegisterMetaType<QwtPlotCurve*>("QwtPlotCurve*");

  m_Panner = new QwtPlotPanner(ui->m_CctwGraph->canvas());
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setEnabled(true);

  m_Zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, ui->m_CctwGraph->canvas());
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);
//  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);

  m_Magnifier = new QwtPlotMagnifier(ui->m_CctwGraph->canvas());
  m_Magnifier -> setMouseFactor(1.0);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
//  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  ui->m_CctwGraph -> insertLegend(m_Legend, QwtPlot::BottomLegend);
}

CctwqtMainWindow::~CctwqtMainWindow()
{
  delete ui;
}

void CctwqtMainWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

void CctwqtMainWindow::possiblyClose()
{
  close();
}

bool CctwqtMainWindow::wantToClose()
{
  THREAD_CHECK;

  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window?"),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void CctwqtMainWindow::printLine(QString line)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "printLine", Q_ARG(QString, line));
  } else {
    ui->m_OutputMessages->append(line);
  }
}

void CctwqtMainWindow::printMessage(QString msg, QDateTime dt)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    ui->m_OutputMessages->append(dt.toString("yyyy MMM dd hh:mm:ss.zzz: ")+msg);
  }
}

void CctwqtMainWindow::doEvaluateCommand()
{
  QString cmd = ui->m_CommandInput->text();

  QMetaObject::invokeMethod(m_Application, "evaluateCommand", Q_ARG(QString, cmd));
}

void CctwqtMainWindow::updateImportImagePaths(QStringList p)
{
  ui->m_ImportDataImages->clear();
  ui->m_ImportDataImages->addItems(p);
}

void CctwqtMainWindow::updateImportChunkSize(CctwIntVector3D sz)
{
  ui->m_ImportOutputChunkX->setValue(sz.x());
  ui->m_ImportOutputChunkY->setValue(sz.y());
  ui->m_ImportOutputChunkZ->setValue(sz.z());
}

void CctwqtMainWindow::doSetupImport()
{
  if (m_SetupImportDialog == NULL) {
    m_SetupImportDialog = new CctwqtSetupImportDialog(this, m_Application->m_ImportData);
    m_SetupImportDialog -> show();
  }

  m_SetupImportDialog->raise();
  m_SetupImportDialog->activateWindow();
}

void CctwqtMainWindow::doImport()
{
  QtConcurrent::run(m_Application->m_ImportData, &CctwImporter::importData);
}

void CctwqtMainWindow::updateInputDimensions()
{
  if (m_Application) {
    CctwChunkedData *inputData = m_Application->m_InputData;

    if (inputData) {
      ui->m_InputDimensionsX->setValue(inputData->dimensions().x());
      ui->m_InputDimensionsY->setValue(inputData->dimensions().y());
      ui->m_InputDimensionsZ->setValue(inputData->dimensions().z());

      ui->m_InputCountX->setValue(inputData->chunkCount().x());
      ui->m_InputCountY->setValue(inputData->chunkCount().y());
      ui->m_InputCountZ->setValue(inputData->chunkCount().z());

      ui->m_InputChunkX->setValue(inputData->chunkSize().x());
      ui->m_InputChunkY->setValue(inputData->chunkSize().y());
      ui->m_InputChunkZ->setValue(inputData->chunkSize().z());
    }
  }
}

void CctwqtMainWindow::updateInputHDF5ChunkSize(CctwIntVector3D sz)
{
  ui->m_InputHDFChunkX->setValue(sz.x());
  ui->m_InputHDFChunkY->setValue(sz.y());
  ui->m_InputHDFChunkZ->setValue(sz.z());
}

void CctwqtMainWindow::doSetupInput()
{
  if (m_SetupInputDialog == NULL) {
    m_SetupInputDialog = new CctwqtSetupInputDialog(this, m_Application->m_InputData);
    m_SetupInputDialog -> show();
  }

  m_SetupInputDialog->raise();
  m_SetupInputDialog->activateWindow();
}

void CctwqtMainWindow::updateOutputDimensions()
{
  if (m_Application) {
    CctwChunkedData *outputData = m_Application->m_OutputData;

    if (outputData) {
      ui->m_OutputDimensionsX->setValue(outputData->dimensions().x());
      ui->m_OutputDimensionsY->setValue(outputData->dimensions().y());
      ui->m_OutputDimensionsZ->setValue(outputData->dimensions().z());

      ui->m_OutputCountX->setValue(outputData->chunkCount().x());
      ui->m_OutputCountY->setValue(outputData->chunkCount().y());
      ui->m_OutputCountZ->setValue(outputData->chunkCount().z());

      ui->m_OutputChunkX->setValue(outputData->chunkSize().x());
      ui->m_OutputChunkY->setValue(outputData->chunkSize().y());
      ui->m_OutputChunkZ->setValue(outputData->chunkSize().z());
    }
  }
}

void CctwqtMainWindow::updateOutputHDF5ChunkSize(CctwIntVector3D sz)
{
  ui->m_OutputHDFChunkX->setValue(sz.x());
  ui->m_OutputHDFChunkY->setValue(sz.y());
  ui->m_OutputHDFChunkZ->setValue(sz.z());
}

void CctwqtMainWindow::doSetupOutput()
{
  if (m_SetupOutputDialog == NULL) {
    m_SetupOutputDialog = new CctwqtSetupOutputDialog(this, m_Application->m_OutputData);
    m_SetupOutputDialog -> show();
  }

  m_SetupOutputDialog->raise();
  m_SetupOutputDialog->activateWindow();
}

void CctwqtMainWindow::doSetupTransform()
{
  if (m_SetupTransformDialog == NULL) {
    m_SetupTransformDialog = new CctwqtSetupTransformDialog(m_Application, this);
    m_SetupTransformDialog -> show();
  }

  m_SetupTransformDialog->raise();
  m_SetupTransformDialog->activateWindow();
}

void CctwqtMainWindow::doTransform()
{
  QtConcurrent::run(m_Application->m_Transformer, &CctwTransformer::transform);
}

void CctwqtMainWindow::doHalt()
{
  m_Application->set_Halting(true);
}

void CctwqtMainWindow::doSaveSettings()
{
  QString path = QFileDialog::getSaveFileName(this, "Save Settings in...",
                                              m_Application->get_SettingsPath());

  if (path.length()) {
    m_Application->writeSettings(path);
  }
}

void CctwqtMainWindow::doLoadSettings()
{
  QString path = QFileDialog::getOpenFileName(this, "Load Settings from...",
                                              m_Application->get_SettingsPath());

  if (path.length()) {
    m_Application->readSettings(path);
  }
}

void CctwqtMainWindow::doSaveDependencies()
{
  QString path = QFileDialog::getSaveFileName(this, "Save Dependencies in...",
                                              m_Application->get_DependenciesPath());

  if (path.length()) {
    m_Application->saveDependencies(path);
  }
}

void CctwqtMainWindow::doLoadDependencies()
{
  QString path = QFileDialog::getOpenFileName(this, "Load Dependencies from...",
                                              m_Application->get_DependenciesPath());

  if (path.length()) {
    m_Application->loadDependencies(path);
  }
}

void CctwqtMainWindow::doAnalyzePEMetaData()
{
  QString path = QFileDialog::getOpenFileName(this, "Spec Data File for PE rotscan runs...",
                                              m_Application->get_SpecDataFilePath());

  if (path.length()) {
    m_Application->analyzePEMetaData(path);
  }
}

void CctwqtMainWindow::doAnalyzeSpecDataFile()
{
  QString path = QFileDialog::getOpenFileName(this, "Spec Data File for PE rotscan runs...",
                                              m_Application->get_SpecDataFilePath());

  if (path.length()) {
    m_Application->analyzeSpecDataFile(path);
  }
}

void CctwqtMainWindow::onProgressUpdate()
{
  int prog = m_Application->get_Progress();
  int lim  = m_Application->get_ProgressLimit();

  ui->m_ProgressBar->setMaximum(lim);
  ui->m_ProgressBar->setValue(prog);
}

void CctwqtMainWindow::plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this,
                              "plotCurves",
                              Q_ARG(QwtPlotCurve*, c1),
                              Q_ARG(QwtPlotCurve*, c2),
                              Q_ARG(QwtPlotCurve*, c3),
                              Q_ARG(QwtPlotCurve*, c4));
  } else {
    c1->setBrush(QBrush(Qt::red));
    c1->setStyle(QwtPlotCurve::NoCurve);
    c1->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::red),QPen(Qt::red),QSize(3,3)));

    c2->setBrush(QBrush(Qt::blue));
    c2->setStyle(QwtPlotCurve::NoCurve);
    c2->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::blue),QPen(Qt::blue),QSize(3,3)));

    c3->setBrush(QBrush(Qt::green));
    c3->setStyle(QwtPlotCurve::NoCurve);
    c3->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::green),QPen(Qt::green),QSize(3,3)));

    c4->setBrush(QBrush(Qt::black));
    c4->setStyle(QwtPlotCurve::NoCurve);
    c4->setSymbol(new QwtSymbol(QwtSymbol::Rect,QBrush(Qt::black),QPen(Qt::black),QSize(3,3)));

    c1 -> attach(ui->m_CctwGraph);
    c2 -> attach(ui->m_CctwGraph);
    c3 -> attach(ui->m_CctwGraph);
    c4 -> attach(ui->m_CctwGraph);

    ui->m_CctwGraph -> setAxisAutoScale(QwtPlot::xBottom);
    ui->m_CctwGraph -> setAxisAutoScale(QwtPlot::yLeft);
    ui->m_CctwGraph -> setAxisAutoScale(QwtPlot::yRight);

    m_Zoomer->setZoomBase();
  }
}

void CctwqtMainWindow::doCompareHDF5()
{
  if (m_SetupCompareDialog == NULL) {
    m_SetupCompareDialog = new CctwqtSetupCompareDialog(this, m_Application->m_CompareData);
    m_SetupCompareDialog -> show();
  }

  m_SetupCompareDialog->raise();
  m_SetupCompareDialog->activateWindow();
}

void CctwqtMainWindow::doCheckImportedData()
{
  if (m_SetupCheckImportDialog == NULL) {
    m_SetupCheckImportDialog = new CctwqtSetupCheckImportDialog(this, m_Application->m_ImportData);
    m_SetupCheckImportDialog ->show();
  }

  m_SetupCheckImportDialog->raise();
  m_SetupCheckImportDialog->activateWindow();
}
