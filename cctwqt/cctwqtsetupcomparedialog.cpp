#include "cctwqtsetupcomparedialog.h"
#include "ui_cctwqtsetupcomparedialog.h"

CctwqtSetupCompareDialog::CctwqtSetupCompareDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupCompareDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

}

CctwqtSetupCompareDialog::~CctwqtSetupCompareDialog()
{
  delete ui;
}
