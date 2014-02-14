#include "cctwqtsetupcomparedialog.h"
#include "ui_cctwqtsetupcomparedialog.h"
#include "cctwqtmainwindow.h"
#include "cctwimportdata.h"
#include <QFileDialog>
#include <QtConcurrentRun>

CctwqtSetupCompareDialog::CctwqtSetupCompareDialog(CctwqtMainWindow *parent, CctwCompareData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupCompareDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  if (m_Data) {
    ui->m_FileName1->setText(m_Data->get_FilePath1());
    ui->m_DatasetName1->setText(m_Data->get_Dataset1());

    ui->m_FileName2->setText(m_Data->get_FilePath2());
    ui->m_DatasetName2->setText(m_Data->get_Dataset2());

    ui->m_CompareRigorously->setChecked(m_Data->get_CompareRigorously());
    ui->m_CompareApproximately->setChecked(m_Data->get_CompareApproximately());
  }

  connect(ui->m_BrowseFile1, SIGNAL(clicked()), this, SLOT(browse1()));
  connect(ui->m_BrowseFile2, SIGNAL(clicked()), this, SLOT(browse2()));
}

CctwqtSetupCompareDialog::~CctwqtSetupCompareDialog()
{
  delete ui;
}

void CctwqtSetupCompareDialog::accept()
{
  if (m_Data) {
    m_Data->set_FilePath1(ui->m_FileName1->text());
    m_Data->set_Dataset1(ui->m_DatasetName1->text());

    m_Data->set_FilePath2(ui->m_FileName2->text());
    m_Data->set_Dataset2(ui->m_DatasetName2->text());

    m_Data->set_CompareRigorously(ui->m_CompareRigorously->isChecked());
    m_Data->set_CompareApproximately(ui->m_CompareApproximately->isChecked());
  }

  QtConcurrent::run(m_Data, &CctwCompareData::compareDatasets);
//  QMetaObject::invokeMethod(m_Data, "compareDatasets");

  QDialog::accept();
}

void CctwqtSetupCompareDialog::browse1()
{
  QString path = QFileDialog::getOpenFileName(this, "File 1", ui->m_FileName1->text());

  if (path.length() > 0) {
    ui->m_FileName1->setText(path);
  }
}

void CctwqtSetupCompareDialog::browse2()
{
  QString path = QFileDialog::getOpenFileName(this, "File 2", ui->m_FileName2->text());

  if (path.length() > 0) {
    ui->m_FileName2->setText(path);
  }
}
