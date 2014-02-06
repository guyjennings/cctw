#include "cctwqtsetupcheckimportdialog.h"
#include "ui_cctwqtsetupcheckimportdialog.h"
#include "cctwqtmainwindow.h"
#include "cctwimportdata.h"
#include <QDir>
#include <QtConcurrentRun>

CctwqtSetupCheckImportDialog::CctwqtSetupCheckImportDialog(CctwqtMainWindow *parent, CctwImportData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupCheckImportDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  if (m_Data) {
    ui->m_DarkImagePath->setText(m_Data->get_DarkImagePath());
    setCurrentDirectory(m_Data->get_ImageDirectory());
    ui->m_DataImages->addItems(m_Data->get_ImagePaths());
    ui->m_OutputPath->setText(m_Data->get_OutputPath());

    ui->m_CheckRigorously->setChecked(m_Data->get_CheckRigorously());
    ui->m_CheckApproximately->setChecked(m_Data->get_CheckApproximately());
  }
}

CctwqtSetupCheckImportDialog::~CctwqtSetupCheckImportDialog()
{
  delete ui;
}

void CctwqtSetupCheckImportDialog::accept()
{
  if (m_Data) {
    m_Data->set_CheckRigorously(ui->m_CheckRigorously->isChecked());
    m_Data->set_CheckApproximately(ui->m_CheckApproximately->isChecked());
  }

  QtConcurrent::run(m_Data, &CctwImportData::checkImportedData);
//  QMetaObject::invokeMethod(m_Data, "checkImportedData", Qt::QueuedConnection);

  QDialog::accept();
}

void CctwqtSetupCheckImportDialog::setCurrentDirectory(QString dir)
{
  ui->m_DataDirectory->setText(dir);
}
