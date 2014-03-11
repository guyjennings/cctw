#include "cctwcomparedata.h"
#include "cctwapplication.h"
#include "cctwinputdatah5.h"

CctwCompareData::CctwCompareData(CctwApplication *application, QString name, QObject *parent) :
  CctwObject(name, parent),
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

static int randomIndex(int n)
{
  return qrand()%n;
}

void CctwCompareData::compareDatasetsApproximately()
{
  printMessage("Compare datasets approximately");

  CctwInputDataH5 d1(get_FilePath1(), get_Dataset1(), "d1", this);
  CctwInputDataH5 d2(get_FilePath2(), get_Dataset2(), "d2", this);

  CctwIntVector3D dim1 = d1.dimensions();
  CctwIntVector3D dim2 = d2.dimensions();

  if (dim1 != dim2) {
    printMessage(tr("Dataset dimensions are not the same (%1 vs %2)")
                 .arg(dim1.toString()).arg(dim2.toString()));

    return;
  }

  QTime startAt;
  startAt.start();
  int n=0;

  while (startAt.elapsed() < 60*1000
         && !m_Application->get_Halting()
         && n < 100) {
    int nx = randomIndex(12)+4;
    int ny = randomIndex(12)+4;
    int nz = randomIndex(12)+4;

    int ix = randomIndex(dim1.x()-nx);
    int iy = randomIndex(dim1.y()-ny);
    int iz = randomIndex(dim1.z()-nz);

    QVector<double> c1 = d1.readChunk(ix, iy, iz, nx, ny, nz);
    QVector<double> c2 = d2.readChunk(ix, iy, iz, nx, ny, nz);

    if (c1.count() != c2.count()) {
      n++;
      printMessage(tr("Chunk data lengths are not the same for chunk %1 :: %2: (%3 vs %4)")
                   .arg(CctwIntVector3D(ix,iy,iz).toString())
                   .arg(CctwIntVector3D(nx,ny,nz).toString())
                   .arg(c1.count()).arg(c2.count()));
    } else {
      for (int i=0; i<c1.count(); i++) {
        if (c1[i] != c2[i]) {
          n++;
          printMessage(tr("Chunk values differ in chunk %1 :: %2: at %3 (%4 vs %5)")
                       .arg(CctwIntVector3D(ix,iy,iz).toString())
                       .arg(CctwIntVector3D(nx,ny,nz).toString())
                       .arg(i).arg(c1[i]).arg(c2[i]));
          break;
        }
      }
    }
  }

  printMessage(tr("Compare completed, %1 errors found").arg(n));
}
