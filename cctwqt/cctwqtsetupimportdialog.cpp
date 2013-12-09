#include "cctwqtsetupimportdialog.h"
#include "ui_cctwqtsetupimportdialog.h"
#include "cctwqtmainwindow.h"

CctwqtSetupImportDialog::CctwqtSetupImportDialog(CctwqtMainWindow *parent) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupImportDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
}

CctwqtSetupImportDialog::~CctwqtSetupImportDialog()
{
  delete ui;
}

void CctwqtSetupImportDialog::accept()
{
  QDialog::accept();
}
