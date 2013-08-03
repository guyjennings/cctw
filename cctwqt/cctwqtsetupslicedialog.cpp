#include "cctwqtsetupslicedialog.h"
#include "ui_cctwqtsetupslicedialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupSliceDialog::CctwqtSetupSliceDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupSliceDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtSetupSliceDialog::~CctwqtSetupSliceDialog()
{
  delete ui;
}

void CctwqtSetupSliceDialog::accept()
{
  QDialog::accept();
}
