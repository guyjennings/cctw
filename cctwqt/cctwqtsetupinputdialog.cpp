#include "cctwqtsetupinputdialog.h"
#include "ui_cctwqtsetupinputdialog.h"

CctwqtSetupInputDialog::CctwqtSetupInputDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupInputDialog)
{
  ui->setupUi(this);
}

CctwqtSetupInputDialog::~CctwqtSetupInputDialog()
{
  delete ui;
}
