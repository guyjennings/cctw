#include "cctwimportdata.h"
#include "cctwapplication.h"
#include <QDir>

CctwImportData::CctwImportData(CctwApplication *application, QObject *parent) :
  CctwObject(parent),
  m_Application(application),
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
  printMessage("Import data not yet implemented");

  foreach (QString s, get_ImagePaths()) {
    printMessage(tr("Import %1").arg(s));
  }
}
