#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"
#include <QLineEdit>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

CctwqtMainWindow::CctwqtMainWindow(CctwqtApplication *app, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CctwqtMainWindow),
  m_Application(app),
  m_SetupInputDialog(NULL),
  m_SetupOutputDialog(NULL),
  m_SetupTransformDialog(NULL),
  m_SetupSliceDialog(NULL),
  m_TransformOneDialog(NULL)
{
  ui->setupUi(this);

  connect(ui->m_CommandInput, SIGNAL(returnPressed()), this, SLOT(doEvaluateCommand()));

  connect(ui->m_SetupInputButton, SIGNAL(clicked()), this, SLOT(doSetupInput()));
  connect(ui->m_SetupOutputButton, SIGNAL(clicked()), this, SLOT(doSetupOutput()));
  connect(ui->m_SetupTransformButton, SIGNAL(clicked()), this, SLOT(doSetupTransform()));
  connect(ui->m_SetupSliceButton, SIGNAL(clicked()), this, SLOT(doSetupSlice()));
  connect(ui->m_TransformAllButton, SIGNAL(clicked()), this, SLOT(doTransformAll()));
  connect(ui->m_TransformOneButton, SIGNAL(clicked()), this, SLOT(doTransformOne()));
  connect(ui->m_TransformSliceButton, SIGNAL(clicked()), this, SLOT(doTransformSlice()));
  connect(ui->m_HaltButton, SIGNAL(clicked()), this, SLOT(doHalt()));
  connect(ui->m_DependenciesButton, SIGNAL(clicked()), m_Application, SLOT(calculateDependencies()));
  connect(ui->m_SaveDepsButton, SIGNAL(clicked()), this, SLOT(doSaveDependencies()));
  connect(ui->m_LoadDepsButton, SIGNAL(clicked()), this, SLOT(doLoadDependencies()));
  connect(ui->m_PEMetaDataButton, SIGNAL(clicked()), this, SLOT(doAnalyzePEMetaData()));

  connect(ui->m_ActionDependencies, SIGNAL(triggered()), m_Application, SLOT(calculateDependencies()));
  connect(ui->m_ActionLoadDependencies, SIGNAL(triggered()), this, SLOT(doLoadDependencies()));
  connect(ui->m_ActionSaveDependencies, SIGNAL(triggered()), this, SLOT(doSaveDependencies()));
  connect(ui->m_ActionReportDependencies, SIGNAL(triggered()), m_Application, SLOT(reportDependencies()));
  connect(ui->m_ActionAnalyzePEMetaData, SIGNAL(triggered()), this, SLOT(doAnalyzePEMetaData()));
  connect(ui->m_ActionLoadSettings, SIGNAL(triggered()), this, SLOT(doLoadSettings()));
  connect(ui->m_ActionSaveSettings, SIGNAL(triggered()), this, SLOT(doSaveSettings()));
  connect(ui->m_ActionQuit, SIGNAL(triggered()), this, SLOT(possiblyClose()));

  app->prop_Halting()->linkTo(ui->m_Halting);
  app->prop_InputDataDescriptor()->linkTo(ui->m_InputData);
  app->prop_OutputDataDescriptor()->linkTo(ui->m_OutputData);
  app->prop_OutputSliceDataDescriptor()->linkTo(ui->m_OutputSliceData);
  app->prop_InverseAvailable()->linkTo(ui->m_InverseAvailable);

  connect(app->prop_Progress(), SIGNAL(valueChanged(int,int)), this, SLOT(onProgressUpdate()));
  connect(app->prop_ProgressLimit(), SIGNAL(valueChanged(int,int)), this, SLOT(onProgressUpdate()));
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

void CctwqtMainWindow::doSetupInput()
{
  if (m_SetupInputDialog == NULL) {
    m_SetupInputDialog = new CctwqtSetupInputDialog(this);
    m_SetupInputDialog -> show();
  }

  m_SetupInputDialog->raise();
  m_SetupInputDialog->activateWindow();
}

void CctwqtMainWindow::doSetupOutput()
{
  if (m_SetupOutputDialog == NULL) {
    m_SetupOutputDialog = new CctwqtSetupOutputDialog(this);
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

void CctwqtMainWindow::doSetupSlice()
{
  if (m_SetupSliceDialog == NULL) {
    m_SetupSliceDialog = new CctwqtSetupSliceDialog(this);
    m_SetupSliceDialog -> show();
  }

  m_SetupSliceDialog->raise();
  m_SetupSliceDialog->activateWindow();
}

void CctwqtMainWindow::doTransformAll()
{
}

void CctwqtMainWindow::doTransformOne()
{
  if (m_TransformOneDialog == NULL) {
    m_TransformOneDialog = new CctwqtTransformOneDialog(this);
    m_TransformOneDialog -> show();
  }

  m_TransformOneDialog->raise();
  m_TransformOneDialog->activateWindow();
}

void CctwqtMainWindow::doTransformSlice()
{

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

void CctwqtMainWindow::onProgressUpdate()
{
  int prog = m_Application->get_Progress();
  int lim  = m_Application->get_ProgressLimit();

  ui->m_ProgressBar->setMaximum(lim);
  ui->m_ProgressBar->setValue(prog);
}
