#include "cctwqtsetuptransformdialog.h"
#include "ui_cctwqtsetuptransformdialog.h"

CctwqtSetupTransformDialog::CctwqtSetupTransformDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupTransformDialog)
{
  ui->setupUi(this);
}

CctwqtSetupTransformDialog::~CctwqtSetupTransformDialog()
{
  delete ui;
}
