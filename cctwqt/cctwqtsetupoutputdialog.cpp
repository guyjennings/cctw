#include "cctwqtsetupoutputdialog.h"
#include "ui_cctwqtsetupoutputdialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupOutputDialog::CctwqtSetupOutputDialog(CctwqtMainWindow *parent, CctwChunkedData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupOutputDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtSetupOutputDialog::~CctwqtSetupOutputDialog()
{
  delete ui;
}

void CctwqtSetupOutputDialog::accept()
{
  QDialog::accept();
}
