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
#include "cctwdatachunk.h"

CctwqtMainWindow::CctwqtMainWindow(CctwApplication *app, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CctwqtMainWindow),
  m_Application(app),
  m_TransformTester(NULL),
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

  connect(ui->m_ActionSetupDataImport, SIGNAL(triggered()), this, SLOT(doSetupImport()));
  connect(ui->m_SetupImportButton, SIGNAL(clicked()), this, SLOT(doSetupImport()));

  connect(ui->m_ActionImportData, SIGNAL(triggered()), this, SLOT(doImport()));
  connect(ui->m_ImportButton, SIGNAL(clicked()), this, SLOT(doImport()));

  connect(ui->m_ActionSetupInputData, SIGNAL(triggered()), this, SLOT(doSetupInput()));
  connect(ui->m_SetupInputButton, SIGNAL(clicked()), this, SLOT(doSetupInput()));

  connect(ui->m_ActionSetupOutputData, SIGNAL(triggered()), this, SLOT(doSetupOutput()));
  connect(ui->m_SetupOutputButton, SIGNAL(clicked()), this, SLOT(doSetupOutput()));

  connect(ui->m_ActionSetupTransform, SIGNAL(triggered()), this, SLOT(doSetupTransform()));
  connect(ui->m_SetupTransformButton, SIGNAL(clicked()), this, SLOT(doSetupTransform()));

  connect(ui->m_ActionPerformTransform, SIGNAL(triggered()), this, SLOT(doTransform()));
  connect(ui->m_TransformButton, SIGNAL(clicked()), this, SLOT(doTransform()));

  connect(ui->m_ActionCheckTransform, SIGNAL(triggered()), this, SLOT(doCheckTransform()));
  connect(ui->m_CheckTransformButton, SIGNAL(clicked()), this, SLOT(doCheckTransform()));

  connect(ui->m_ActionDummyTransform, SIGNAL(triggered()), this, SLOT(doDummyTransform()));
  connect(ui->m_DummyTransformButton, SIGNAL(clicked()), this, SLOT(doDummyTransform()));

  connect(ui->m_HaltButton, SIGNAL(clicked()), this, SLOT(doHalt()));

  connect(ui->m_ActionDependencies, SIGNAL(triggered()), m_Application, SLOT(calculateDependencies()));
  connect(ui->m_DependenciesButton, SIGNAL(clicked()), m_Application, SLOT(calculateDependencies()));

  connect(ui->m_ActionReportInputDependencies, SIGNAL(triggered()), this, SLOT(reportInputDependencies()));
  connect(ui->m_ReportInputDepsButton, SIGNAL(clicked()), this, SLOT(reportInputDependencies()));

  connect(ui->m_ActionReportOutputDependencies, SIGNAL(triggered()), this, SLOT(reportOutputDependencies()));
  connect(ui->m_ReportOutputDepsButton, SIGNAL(clicked()), this, SLOT(reportOutputDependencies()));

  connect(ui->m_ActionSaveDependencies, SIGNAL(triggered()), this, SLOT(doSaveDependencies()));
  connect(ui->m_SaveDepsButton, SIGNAL(clicked()), this, SLOT(doSaveDependencies()));

  connect(ui->m_ActionLoadDependencies, SIGNAL(triggered()), this, SLOT(doLoadDependencies()));
  connect(ui->m_LoadDepsButton, SIGNAL(clicked()), this, SLOT(doLoadDependencies()));

  connect(ui->m_ActionAnalyzePEMetaData, SIGNAL(triggered()), this, SLOT(doAnalyzePEMetaData()));
  connect(ui->m_PEMetaDataButton, SIGNAL(clicked()), this, SLOT(doAnalyzePEMetaData()));

  connect(ui->m_ActionAnalyseSpecDataFile, SIGNAL(triggered()), this, SLOT(doAnalyzeSpecDataFile()));
  connect(ui->m_AnalyzeSpecDataButton, SIGNAL(clicked()), this, SLOT(doAnalyzeSpecDataFile()));

  connect(ui->m_ActionCompareTwoHDF5, SIGNAL(triggered()), this, SLOT(doCompareHDF5()));
  connect(ui->m_ActionCheckImportedData, SIGNAL(triggered()), this, SLOT(doCheckImportedData()));

  connect(ui->m_ActionLoadSettings, SIGNAL(triggered()), this, SLOT(doLoadSettings()));
  connect(ui->m_ActionSaveSettings, SIGNAL(triggered()), this, SLOT(doSaveSettings()));
  connect(ui->m_ActionSaveCurrentSettings, SIGNAL(triggered()), m_Application, SLOT(writeSettings()));

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

    import->prop_ChunkSize()->linkTo(ui->m_ImportOutputChunkX,
                                     ui->m_ImportOutputChunkY,
                                     ui->m_ImportOutputChunkZ);

    updateImportImagePaths(import->get_ImagePaths());
  }

  CctwChunkedData *inputData = app->m_InputData;

  if (inputData) {
    inputData->prop_DataFileName()->linkTo(ui->m_InputDataFileName);
    inputData->prop_DataSetName()->linkTo(ui->m_InputDataSetName);
    inputData->prop_ChunksRead()->linkTo(ui->m_ChunksRead);

    inputData->prop_Compression()->linkTo(ui->m_InputCompression);
    inputData->prop_ChunksRead()->linkTo(ui->m_ChunksRead);

    inputData->prop_Dimensions()->linkTo(ui->m_InputDimensionsX,
                                         ui->m_InputDimensionsY,
                                         ui->m_InputDimensionsZ);

    inputData->prop_ChunkSize()->linkTo(ui->m_InputChunkX,
                                        ui->m_InputChunkY,
                                        ui->m_InputChunkZ);

    inputData->prop_ChunkCount()->linkTo(ui->m_InputCountX,
                                         ui->m_InputCountY,
                                         ui->m_InputCountZ);

    inputData->prop_HDFChunkSize()->linkTo(ui->m_InputHDFChunkX,
                                           ui->m_InputHDFChunkY,
                                           ui->m_InputHDFChunkZ);
  }

  CctwChunkedData *outputData = app->m_OutputData;

  if (outputData) {
    outputData->prop_DataFileName()->linkTo(ui->m_OutputDataFileName);
    outputData->prop_DataSetName()->linkTo(ui->m_OutputDataSetName);
    outputData->prop_ChunksWritten()->linkTo(ui->m_ChunksWritten);
    outputData->prop_ChunksHeld()->linkTo(ui->m_ChunksHeld);
    outputData->prop_ChunksHeldMax()->linkTo(ui->m_ChunksHeldMax);

    outputData->prop_Compression()->linkTo(ui->m_OutputCompression);
    outputData->prop_ChunksWritten()->linkTo(ui->m_ChunksWritten);

    outputData->prop_Dimensions()->linkTo(ui->m_OutputDimensionsX,
                                          ui->m_OutputDimensionsY,
                                          ui->m_OutputDimensionsZ);

    outputData->prop_ChunkSize()->linkTo(ui->m_OutputChunkX,
                                         ui->m_OutputChunkY,
                                         ui->m_OutputChunkZ);

    outputData->prop_ChunkCount()->linkTo(ui->m_OutputCountX,
                                          ui->m_OutputCountY,
                                          ui->m_OutputCountZ);

    outputData->prop_HDFChunkSize()->linkTo(ui->m_OutputHDFChunkX,
                                            ui->m_OutputHDFChunkY,
                                            ui->m_OutputHDFChunkZ);
  }

  CctwTransformer *xform = app->m_Transformer;

  if (xform) {
    xform->prop_ProjectX()->linkTo(ui->m_ProjectX);
    xform->prop_ProjectY()->linkTo(ui->m_ProjectY);
    xform->prop_ProjectZ()->linkTo(ui->m_ProjectZ);
    xform->prop_ProjectDestination()->linkTo(ui->m_ProjectDestination);
    xform->prop_OversampleX()->linkTo(ui->m_OversampleX);
    xform->prop_OversampleY()->linkTo(ui->m_OversampleY);
    xform->prop_OversampleZ()->linkTo(ui->m_OversampleZ);
  }

  connect(ui->m_ActionProjectBrowse, SIGNAL(triggered()), this, SLOT(doBrowseProject()));
  connect(ui->m_ProjectBrowseButton, SIGNAL(clicked()), this, SLOT(doBrowseProject()));

  connect(ui->m_ActionProjectInput, SIGNAL(triggered()), this, SLOT(doProjectInput()));
  connect(ui->m_ProjectInputButton, SIGNAL(clicked()), this, SLOT(doProjectInput()));

  connect(ui->m_ActionProjectOutput, SIGNAL(triggered()), this, SLOT(doProjectOutput()));
  connect(ui->m_ProjectOutputButton, SIGNAL(clicked()), this, SLOT(doProjectOutput()));

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

  m_TransformTester = new CctwqtTransformTester(this, app->m_Parameters, this);
}

CctwqtMainWindow::~CctwqtMainWindow()
{
  delete ui;
}

CctwApplication *CctwqtMainWindow::cctwApplication()
{
  return m_Application;
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

void CctwqtMainWindow::doSetupInput()
{
  if (m_SetupInputDialog == NULL) {
    m_SetupInputDialog = new CctwqtSetupInputDialog(this, m_Application->m_InputData);
    m_SetupInputDialog -> show();
  }

  m_SetupInputDialog->raise();
  m_SetupInputDialog->activateWindow();
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

void CctwqtMainWindow::doCheckTransform()
{
  QtConcurrent::run(m_Application->m_Transformer, &CctwTransformer::checkTransform);
}

void CctwqtMainWindow::doDummyTransform()
{
  QtConcurrent::run(m_Application->m_Transformer, &CctwTransformer::dummyTransform1);
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
    m_Application->set_SettingsPath(path);
  }
}

void CctwqtMainWindow::doLoadSettings()
{
  QString path = QFileDialog::getOpenFileName(this, "Load Settings from...",
                                              m_Application->get_SettingsPath());

  if (path.length()) {
    m_Application->readSettings(path);
    m_Application->set_SettingsPath(path);
  }
}

void CctwqtMainWindow::doSaveDependencies()
{
  QString path = QFileDialog::getSaveFileName(this, "Save Dependencies in...",
                                              m_Application->get_DependenciesPath());

  if (path.length()) {
    m_Application->saveDependencies(path);
    m_Application->set_DependenciesPath(path);
  }
}

void CctwqtMainWindow::doLoadDependencies()
{
  QString path = QFileDialog::getOpenFileName(this, "Load Dependencies from...",
                                              m_Application->get_DependenciesPath());

  if (path.length()) {
    m_Application->loadDependencies(path);
    m_Application->set_DependenciesPath(path);
  }
}

void CctwqtMainWindow::reportDependencies(CctwChunkedData *data, QString title)
{
  if (data) {
    int maxdeps = 0;
    int ndeps   = 0;
    int sumdeps = 0;
    int nchnk   = data->chunkCount().volume();

    for (int i=0; i < nchnk; i++) {
      CctwDataChunk *chunk = data->chunk(i);

      if (chunk) {
        int ct = chunk->dependencyCount();

        if (ct >= 1) {
          ndeps++;
        }

        if (ct > maxdeps) {
          maxdeps = ct;
        }

        sumdeps += ct;
      }
    }

    ui->m_DependenciesTable->clear();
    ui->m_DependenciesTable->setRowCount(ndeps + 3);
    ui->m_DependenciesTable->setColumnCount((maxdeps+1)>4 ? (maxdeps+1):4);

    ui->m_DependenciesTable->setItem(0, 0, new QTableWidgetItem(title));
    ui->m_DependenciesTable->setItem(1, 0, new QTableWidgetItem(tr("%1 Deps").arg(ndeps)));
    ui->m_DependenciesTable->setItem(1, 1, new QTableWidgetItem(tr("%1 Max").arg(maxdeps)));
    ui->m_DependenciesTable->setItem(1, 2, new QTableWidgetItem(tr("%1 Sum").arg(sumdeps)));
    ui->m_DependenciesTable->setItem(1, 3, new QTableWidgetItem(tr("%1 Avg").arg(double(sumdeps)/double(nchnk))));
    ui->m_DependenciesTable->setItem(1, 4, new QTableWidgetItem(tr("%1 Avg-NZ").arg(double(sumdeps)/double(ndeps))));

    int r = 0;

    for (int i=0; i < nchnk; i++) {
      CctwDataChunk *chunk = data->chunk(i);

      if (chunk) {
        int ct = chunk->dependencyCount();

        if (ct >= 1) {
          ui->m_DependenciesTable->setItem(r+3, 0, new QTableWidgetItem(tr("%1->[%2]").arg(i).arg(ct)));

          for (int i=0; i<ct; i++) {
            ui->m_DependenciesTable->setItem(r+3, 1+i, new QTableWidgetItem(tr("%1").arg(chunk->dependency(i))));
          }

          r++;
        }
      }
    }
  }
}

void CctwqtMainWindow::reportInputDependencies()
{
  reportDependencies(m_Application->m_InputData, "Input Dependencies");
}

void CctwqtMainWindow::reportOutputDependencies()
{
  reportDependencies(m_Application->m_OutputData, "Output Dependencies");
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

void CctwqtMainWindow::doBrowseProject()
{
  CctwTransformer *xform = m_Application->m_Transformer;

  if (xform) {
    QString s = QFileDialog::getExistingDirectory(this, "Destination", xform->get_ProjectDestination());

    if (s.length()) {
      xform->set_ProjectDestination(s);
    }
  }
}

void CctwqtMainWindow::doProjectInput()
{
  CctwTransformer *xform = m_Application->m_Transformer;

  if (xform) {
    int flags = (xform->get_ProjectX() ? 1 : 0) +
                (xform->get_ProjectY() ? 2 : 0) +
                (xform->get_ProjectZ() ? 4 : 0);

    QString inputPath = m_Application->m_InputData->get_DataFileName();

    QFileInfo info(inputPath);

    QString dst = xform->get_ProjectDestination() + "/" + info.completeBaseName();
    QString cmd = tr("transformer.projectInput(\"%1\", %2)").arg(dst).arg(flags);

    m_Application->evaluateCommand(cmd);
  }
}

void CctwqtMainWindow::doProjectOutput()
{
  CctwTransformer *xform = m_Application->m_Transformer;

  if (xform) {
    int flags = (xform->get_ProjectX() ? 1 : 0) +
                (xform->get_ProjectY() ? 2 : 0) +
                (xform->get_ProjectZ() ? 4 : 0);

    QString outputPath = m_Application->m_OutputData->get_DataFileName();

    QFileInfo info(outputPath);

    QString dst = xform->get_ProjectDestination() + "/" + info.completeBaseName();
    QString cmd = tr("transformer.projectOutput(\"%1\", %2)").arg(dst).arg(flags);

    m_Application->evaluateCommand(cmd);
  }
}
