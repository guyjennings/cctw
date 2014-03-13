#include "cctwqtsetupinputdialog.h"
#include "ui_cctwqtsetupinputdialog.h"
#include "cctwqtmainwindow.h"
#include <QFileDialog>
#include "hdf5.h"

CctwqtSetupInputDialog::CctwqtSetupInputDialog(CctwqtMainWindow *parent, CctwChunkedData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupInputDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  connect(ui->m_BrowseInputFile, SIGNAL(clicked()), this, SLOT(doBrowseInputFile()));
  connect(ui->m_InputFileName, SIGNAL(textChanged(QString)), this, SLOT(doCheckDataFile(QString)));
  connect(ui->m_BrowseInputDataset, SIGNAL(currentIndexChanged(int)), this, SLOT(doBrowseInputDataset(int)));
  connect(ui->m_InputDatasetName, SIGNAL(textChanged(QString)), this, SLOT(doCheckDataset(QString)));

  if (m_Data) {
    ui->m_InputFileName->setText(m_Data->get_DataFileName());
    ui->m_InputDatasetName->setText(m_Data->get_DataSetName());
  }
}

CctwqtSetupInputDialog::~CctwqtSetupInputDialog()
{
  delete ui;
}

void CctwqtSetupInputDialog::accept()
{
  if (m_Data) {
    m_Data->set_DataFileName(ui->m_InputFileName->text());
    m_Data->set_DataSetName(ui->m_InputDatasetName->text());
  }

  QDialog::accept();
}

void CctwqtSetupInputDialog::doBrowseInputFile()
{
  if (m_Window) {
    QString path = QFileDialog::getOpenFileName(this, "Input File",
                                              ui->m_InputFileName->text());

    if (path.length() > 0) {
      ui->m_InputFileName->setText(path);
    }
  }
}

static herr_t dataset(hid_t loc_id, const char *name, void *opdata)
{
  H5G_stat_t statbuf;

  /*
   * Get type of the object and display its name and type.
   * The name of the object is passed to this function by
   * the Library. Some magic :-)
   */
  H5Gget_objinfo(loc_id, name, false, &statbuf);

  switch (statbuf.type) {
  case H5G_GROUP:
//       printf(" Object with name %s is a group \n", name);
       break;
  case H5G_DATASET:
//       printf(" Object with name %s is a dataset \n", name);
       if (opdata) {
         QStringList *sl = (QStringList*)(opdata);

         sl->append(name);
       }
       break;
  case H5G_TYPE:
//       printf(" Object with name %s is a named datatype \n", name);
       break;
  default:
       printf(" Unable to identify an object ");
  }

  return 0;
}

static QStringList iterateHDF5datasets(QString path)
{
  /* Save old error handler */
  H5E_auto2_t  old_func;
  void *old_client_data;

  H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

  /* Turn off error handling */
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);

  QStringList paths;

  hid_t file = H5Fopen(qPrintable(path), H5F_ACC_RDONLY, H5P_DEFAULT);

  H5Giterate(file, "/", NULL, dataset, &paths);

  /* Close file */
  herr_t ret = H5Fclose(file);

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return paths;
}

void CctwqtSetupInputDialog::doCheckDataFile(QString path)
{
  if (m_Window) {
    m_Window->printMessage(tr("CctwqtSetupInputDialog::doCheckDataFile(\"%1\")").arg(path));
  }

  QStringList paths = iterateHDF5datasets(path);

  ui->m_BrowseInputDataset->clear();
  ui->m_BrowseInputDataset->addItems(paths);
}

void CctwqtSetupInputDialog::doBrowseInputDataset(int entry)
{
  if (m_Window) {
    m_Window->printMessage(tr("CctwqtSetupInputDialog::doBrowseInputDataset(%1)").arg(entry));
  }
}

void CctwqtSetupInputDialog::doCheckDataset(QString name)
{
  if (m_Window) {
    m_Window->printMessage(tr("CctwqtSetupInputDialog::doCheckDataset(\"%1\")").arg(name));
  }
}
