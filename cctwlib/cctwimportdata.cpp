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

    printMessage(tr("Importing %1 frames of data").arg(n));

    for (int i=0; i<n; i++) {
      m_BacklogSemaphore.acquire(1);
//      printMessage("acquired backlog semaphore");
      QtConcurrent::run(this, &CctwImportData::importDataFrame,
                                     i, paths[i]);
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

  if (path.length() > 0) {

    QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

    if (m.readImage(path)) {
      m.loadMetaData();

      writeOutputFrame(num, &m);
    }
  }

  m_CompletionSemaphore.release(1);
}
