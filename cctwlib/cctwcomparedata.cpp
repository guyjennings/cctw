#include "cctwcomparedata.h"
#include "cctwapplication.h"

CctwCompareData::CctwCompareData(CctwApplication *application, QObject *parent) :
  CctwObject(parent),
  m_Application(application),
  m_FilePath1(m_Application->saver(), this, "filePath1", "", "Path to 1st file"),
  m_Dataset1(m_Application->saver(), this, "dataset1", "data", "Name of dataset within 1st file"),
  m_FilePath2(m_Application->saver(), this, "filePath2", "", "Path to 2nd file"),
  m_Dataset2(m_Application->saver(), this, "dataset2", "data", "Name of dataset within 2nd file"),
  m_CompareRigorously(m_Application->saver(), this, "compareRigorously", false, "Perform rigorous compare (every element) of imported data"),
  m_CompareApproximately(m_Application->saver(), this, "compareApproximately", true, "Perform approximate compare (about 30 seconds) of imported data")
{
}

CctwCompareData::~CctwCompareData()
{
}

void CctwCompareData::compareDatasets()
{
  printMessage("Compare datasets");

  if (get_CompareRigorously()) {
    compareDatasetsRigorously();
  }

  if (get_CompareApproximately()) {
    compareDatasetsApproximately();
  }
}

void CctwCompareData::compareDatasetsRigorously()
{
  printMessage("Compare datasets rigorously");
}

void CctwCompareData::compareDatasetsApproximately()
{
  printMessage("Compare datasets approximately");
}
