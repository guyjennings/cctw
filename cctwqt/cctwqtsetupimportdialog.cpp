#include "cctwqtsetupimportdialog.h"
#include "ui_cctwqtsetupimportdialog.h"
#include "cctwqtmainwindow.h"
#include <QFileDialog>

CctwqtSetupImportDialog::CctwqtSetupImportDialog(CctwqtMainWindow *parent, CctwImportData *data) :
  QDialog(parent),
  ui(new Ui::CctwqtSetupImportDialog),
  m_Window(parent),
  m_Data(data)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  connect(ui->m_AppendImages, SIGNAL(clicked()), this, SLOT(doAppendImages()));
  connect(ui->m_ClearImages, SIGNAL(clicked()), this, SLOT(doClearImages()));
  connect(ui->m_DataDirectoryBrowse, SIGNAL(clicked()), this, SLOT(doBrowseDirectory()));
  connect(ui->m_MatchImages, SIGNAL(clicked()), this, SLOT(doMatchImages()));
  connect(ui->m_DarkImageBrowse, SIGNAL(clicked()), this, SLOT(doBrowseDark()));

  if (m_Data) {
    ui->m_DarkImagePath->setText(m_Data->get_DarkImagePath());
    setCurrentDirectory(m_Data->get_ImageDirectory());
    ui->m_DataImages->addItems(m_Data->get_ImagePaths());
    ui->m_DataPattern->setText(m_Data->get_ImagePattern());
  }
}

CctwqtSetupImportDialog::~CctwqtSetupImportDialog()
{
  delete ui;
}

void CctwqtSetupImportDialog::accept()
{
  if (m_Data) {
    m_Data->set_DarkImagePath(ui->m_DarkImagePath->text());
    m_Data->set_ImageDirectory(currentDirectory());

    QStringList res;

    int ct = ui->m_DataImages->count();

    for (int i=0; i<ct; i++) {
      QListWidgetItem *item = ui->m_DataImages->item(i);

      if (item) {
        res.append(item->text());
      }
    }

    m_Data->set_ImagePaths(res);
    m_Data->set_ImagePattern(ui->m_DataPattern->text());
  }

  QDialog::accept();
}

void CctwqtSetupImportDialog::doAppendImages()
{
  QStringList paths = QFileDialog::getOpenFileNames(this, "Additional images...",
                                                    currentDirectory());

  ui->m_DataImages->addItems(paths);
}

void CctwqtSetupImportDialog::doClearImages()
{
  ui->m_DataImages->clear();
}

void CctwqtSetupImportDialog::doBrowseDirectory()
{
  QString path = QFileDialog::getExistingDirectory(this, "Image Directory",
                                                   currentDirectory());

  if (path.length() > 0) {
    setCurrentDirectory(path);
  }
}

void CctwqtSetupImportDialog::doBrowseDark()
{
  QString path = QFileDialog::getOpenFileName(this, "Dark Image",
                                              ui->m_DarkImagePath->text());

  if (path.length() > 0) {
    ui->m_DarkImagePath->setText(path);
  }
}

void CctwqtSetupImportDialog::doMatchImages()
{
  QString patt = ui->m_DataPattern->text();

  QDir d(currentDirectory());
  d.setFilter(QDir::Files);

  QStringList patts(patt);
  d.setNameFilters(patts);

  QStringList newFiles = d.entryList();

  ui->m_DataImages->addItems(newFiles);
}

QString CctwqtSetupImportDialog::currentDirectory()
{
  return ui->m_DataDirectory->currentText();
}

void CctwqtSetupImportDialog::appendDirectoryPaths(QString dir)
{
  printf("Append directory path %s\n", qPrintable(dir));

  if (dir.length() > 0) {
    QDir d(dir);
    QString p1 = d.absolutePath();

    if (d.cd("..")) {
      QString p = d.absolutePath();

      if ((p.length() > 0) && (p != p1)) {
        appendDirectoryPaths(p);
      }
    }

    ui->m_DataDirectory->addItem(p1);
  }
}

void CctwqtSetupImportDialog::setCurrentDirectory(QString dir)
{
//  disconnect(ui->m_DataDirectory, SIGNAL(currentIndexChanged(int)), this, SLOT(doBrowseDirectoryChanged(int)));
//  bool t = ui->m_DataDirectory->blockSignals(true);

  ui->m_DataDirectory->clear();

  appendDirectoryPaths(dir);

//  ui->m_DataDirectory->blockSignals(t);

//  connect(ui->m_DataDirectory, SIGNAL(currentIndexChanged(int)), this, SLOT(doBrowseDirectoryChanged(int)));
  ui->m_DataDirectory->setCurrentIndex(ui->m_DataDirectory->count()-1);
}

void CctwqtSetupImportDialog::doBrowseDirectoryChanged(int n)
{
  setCurrentDirectory(ui->m_DataDirectory->itemText(n));
}
