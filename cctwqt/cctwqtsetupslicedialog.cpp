#include "cctwqtsetupslicedialog.h"
#include "ui_cctwqtsetupslicedialog.h"

CctwqtSetupSliceDialog::CctwqtSetupSliceDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupSliceDialog)
{
  ui->setupUi(this);
}

CctwqtSetupSliceDialog::~CctwqtSetupSliceDialog()
{
  delete ui;
}
