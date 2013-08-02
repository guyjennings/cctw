#include "cctwqtsetupoutputdialog.h"
#include "ui_cctwqtsetupoutputdialog.h"

CctwqtSetupOutputDialog::CctwqtSetupOutputDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupOutputDialog)
{
  ui->setupUi(this);
}

CctwqtSetupOutputDialog::~CctwqtSetupOutputDialog()
{
  delete ui;
}
