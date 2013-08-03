#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"
#include <QLineEdit>
#include <QThread>

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
}

CctwqtMainWindow::~CctwqtMainWindow()
{
  delete ui;
}

void CctwqtMainWindow::printMessage(QString msg)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "printMessage", Q_ARG(QString, msg));
  } else {
    ui->m_OutputMessages->append(msg);
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
    m_SetupTransformDialog = new CctwqtSetupTransformDialog(this);
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

}

