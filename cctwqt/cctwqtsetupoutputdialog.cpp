#include "cctwqtsetupoutputdialog.h"
#include "ui_cctwqtsetupoutputdialog.h"
#include "cctwqtmainwindow.h"
#include <QFileDialog>

CctwqtSetupOutputDialog::CctwqtSetupOutputDialog(CctwqtMainWindow *parent, CctwChunkedData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupOutputDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  connect(ui->m_BrowseOutputFileName, SIGNAL(clicked()), this, SLOT(doBrowseOutputFile()));

  if(m_Data) {
    ui->m_OutputFileName->setText(m_Data->get_DataFileName());
    ui->m_OutputDatasetName->setText(m_Data->get_DataSetName());
    ui->m_OutputCompression->setValue(m_Data->get_Compression());

    CctwIntVector3D cksz = m_Data->chunkSize();

    ui->m_OutputChunkX->setValue(cksz.x());
    ui->m_OutputChunkY->setValue(cksz.y());
    ui->m_OutputChunkZ->setValue(cksz.z());
  }
}

CctwqtSetupOutputDialog::~CctwqtSetupOutputDialog()
{
  delete ui;
}

void CctwqtSetupOutputDialog::accept()
{
  if (m_Data) {
    CctwIntVector3D cksz(ui->m_OutputChunkX->value(),
                         ui->m_OutputChunkY->value(),
                         ui->m_OutputChunkZ->value());

    m_Data->setChunkSize(cksz);
    m_Data->set_Compression(ui->m_OutputCompression->value());

    m_Data->set_DataFileName(ui->m_OutputFileName->text());
    m_Data->set_DataSetName(ui->m_OutputDatasetName->text());
  }

  QDialog::accept();
}

void CctwqtSetupOutputDialog::doBrowseOutputFile()
{
  if (m_Window) {
    QString path = QFileDialog::getSaveFileName(this, "Output File",
                                              ui->m_OutputFileName->text());

    if (path.length() > 0) {
      ui->m_OutputFileName->setText(path);
    }
  }
}
