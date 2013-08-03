#include "cctwqtsetupinputdialog.h"
#include "ui_cctwqtsetupinputdialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupInputDialog::CctwqtSetupInputDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupInputDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtSetupInputDialog::~CctwqtSetupInputDialog()
{
  delete ui;
}

void CctwqtSetupInputDialog::accept()
{
  QDialog::accept();
}
