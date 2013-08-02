#include "cctwqttransformonedialog.h"
#include "ui_cctwqttransformonedialog.h"

CctwqtTransformOneDialog::CctwqtTransformOneDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtTransformOneDialog)
{
  ui->setupUi(this);
}

CctwqtTransformOneDialog::~CctwqtTransformOneDialog()
{
  delete ui;
}
