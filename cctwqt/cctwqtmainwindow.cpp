#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"
#include <QLineEdit>
#include <QThread>

CctwqtMainWindow::CctwqtMainWindow(CctwqtApplication *app, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CctwqtMainWindow),
  m_Application(app)
{
  ui->setupUi(this);

  connect(ui->m_CommandInput, SIGNAL(returnPressed()), this, SLOT(doEvaluateCommand()));
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
