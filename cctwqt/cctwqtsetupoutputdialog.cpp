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

    CctwIntVector3D dims = m_Data->dimensions();

    ui->m_OutputDimensionsX->setValue(dims.x());
    ui->m_OutputDimensionsY->setValue(dims.y());
    ui->m_OutputDimensionsZ->setValue(dims.z());

    CctwIntVector3D cksz = m_Data->chunkSize();

    ui->m_OutputChunkX->setValue(cksz.x());
    ui->m_OutputChunkY->setValue(cksz.y());
    ui->m_OutputChunkZ->setValue(cksz.z());

    CctwIntVector3D hcksz = m_Data->get_HDFChunkSize();

    ui->m_HDFOutputChunkX->setValue(hcksz.x());
    ui->m_HDFOutputChunkY->setValue(hcksz.y());
    ui->m_HDFOutputChunkZ->setValue(hcksz.z());
  }
}

CctwqtSetupOutputDialog::~CctwqtSetupOutputDialog()
{
  delete ui;
}

void CctwqtSetupOutputDialog::accept()
{
  if (m_Data) {
    CctwIntVector3D dims(ui->m_OutputDimensionsX->value(),
                         ui->m_OutputDimensionsY->value(),
                         ui->m_OutputDimensionsZ->value());

    CctwIntVector3D cksz(ui->m_OutputChunkX->value(),
                         ui->m_OutputChunkY->value(),
                         ui->m_OutputChunkZ->value());

    CctwIntVector3D hcksz(ui->m_HDFOutputChunkX->value(),
                          ui->m_HDFOutputChunkY->value(),
                          ui->m_HDFOutputChunkZ->value());

    m_Data->setDimensions(dims);
    m_Data->setChunkSize(cksz);
    m_Data->set_Compression(ui->m_OutputCompression->value());

    m_Data->set_DataFileName(ui->m_OutputFileName->text());
    m_Data->set_DataSetName(ui->m_OutputDatasetName->text());

    m_Data->set_HDFChunkSize(hcksz);
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
