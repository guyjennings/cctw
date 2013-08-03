#include "cctwqttransformonedialog.h"
#include "ui_cctwqttransformonedialog.h"
#include "cctwqtmainwindow.h"

CctwqtTransformOneDialog::CctwqtTransformOneDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtTransformOneDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtTransformOneDialog::~CctwqtTransformOneDialog()
{
  delete ui;
}

void CctwqtTransformOneDialog::accept()
{
  QDialog::accept();
}
