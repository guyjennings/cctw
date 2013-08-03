#include "cctwqtsetuptransformdialog.h"
#include "ui_cctwqtsetuptransformdialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupTransformDialog::CctwqtSetupTransformDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupTransformDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtSetupTransformDialog::~CctwqtSetupTransformDialog()
{
  delete ui;
}

void CctwqtSetupTransformDialog::accept()
{
  QDialog::accept();
}
