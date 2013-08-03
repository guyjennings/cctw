#include "cctwqtsetupoutputdialog.h"
#include "ui_cctwqtsetupoutputdialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupOutputDialog::CctwqtSetupOutputDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupOutputDialog)
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
