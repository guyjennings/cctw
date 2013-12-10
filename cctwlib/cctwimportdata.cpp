#include "cctwimportdata.h"
#include "cctwapplication.h"
#include <QDir>
#include <QtConcurrentRun>
#include "qcepmutexlocker.h"

CctwImportData::CctwImportData(CctwApplication *application, QObject *parent) :
  CctwObject(parent),
  m_Application(application),
  m_BacklogSemaphore(8),
  m_CompletionSemaphore(0),
  m_DataFormat(m_Application->saver(), this, "dataFormat", 0, "Imported data format (0=auto)"),
  m_DarkImagePath(m_Application->saver(), this, "darkImagePath", "", "Dark image path"),
  m_ImagePaths(m_Application->saver(), this, "imagePaths", QcepStringList(), "Imported image paths"),
  m_ImageDirectory(m_Application->saver(), this, "imageDirectory", "", "Image directory"),
  m_ImagePattern(m_Application->saver(), this, "imagePattern", "", "Image pattern"),
  m_OutputPath(m_Application->saver(), this, "outputPath", "", "Destination for imported data")
{
}

void CctwImportData::clearInputFiles()
{
  prop_ImagePaths()->clear();
}

void CctwImportData::changeDirectory(QString path)
{
  set_ImageDirectory(path);
}

void CctwImportData::appendInputFile(QString path)
{
  prop_ImagePaths()->appendValue(path);
}

void CctwImportData::appendMatchingFiles(QString pattern)
{
  set_ImagePattern(pattern);

  QDir d(get_ImageDirectory());
  d.setFilter(QDir::Files);

  QStringList patts(pattern);
  d.setNameFilters(patts);

  QStringList newFiles = d.entryList();

  QStringList l = get_ImagePaths();

  l.append(newFiles);

  set_ImagePaths(l);
}

void CctwImportData::importData()
{
  if (createOutputFile()) {

    QStringList paths = get_ImagePaths();
    int n = paths.count();
    QVector< QFuture<void> > results(n);
    QDir inp(get_ImageDirectory());

    if (m_Application) {
      m_Application->waitCompleted();
      m_Application->set_Progress(0);
      m_Application->set_Halting(false);
      m_Application->set_ProgressLimit(n);
    }

    printMessage(tr("Importing %1 frames of data").arg(n));

    for (int i=0; i<n; i++) {
      if (m_Application) {
        m_Application->addWorkOutstanding(1);
      }

      m_BacklogSemaphore.acquire(1);
//      printMessage("acquired backlog semaphore");
      QtConcurrent::run(this, &CctwImportData::importDataFrame,
                                     i, inp.filePath(paths[i]));
    }

    // waitTillFinished();

    m_CompletionSemaphore.acquire(n);
    closeOutputFile();
    m_CompletionSemaphore.release(n);

    printMessage("Import Completed");
  }
}

bool CctwImportData::createOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  return true;
}

void CctwImportData::closeOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);
}

void CctwImportData::writeOutputFrame(int num, QcepImageData<double> *img)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  printMessage(tr("Writing output frame %1").arg(num));
}

void CctwImportData::importDataFrame(int num, QString path)
{
  m_BacklogSemaphore.release(1);

  if (m_Application && !m_Application->get_Halting()) {
    if (path.length() > 0) {

      QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

      if (m.readImage(path)) {
        m.loadMetaData();

        printMessage(tr("Imported frame %1 from %2").arg(num).arg(path));

        writeOutputFrame(num, &m);
      }
    }
  }

  m_CompletionSemaphore.release(1);

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}
