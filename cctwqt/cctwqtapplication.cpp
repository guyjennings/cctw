#include "cctwqtapplication.h"
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwqtoutputslicedata.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwtransformer.h"
#include <QScriptValue>
#include "cctwqtscriptengine.h"
#include "qcepproperty.h"
#include "cctwqtdebug.h"
#include "qcepsettingssaver.h"
#include <QtConcurrentRun>
#include <QFile>

QcepSettingsSaverPtr g_Saver;

CctwqtApplication::CctwqtApplication(int &argc, char *argv[]) :
  QApplication(argc, argv),
  m_ObjectNamer(this, "cctw"),
  m_Window(NULL),
  m_InputDataManager(NULL),
  m_InputData(NULL),
  m_OutputDataManager(NULL),
  m_OutputData(NULL),
  m_OutputSliceDataManager(NULL),
  m_OutputSliceData(NULL),
  m_Transform(NULL),
  m_Transformer(NULL),
  m_SliceTransform(NULL),
  m_SliceTransformer(NULL),
  m_ScriptEngine(NULL),
  m_TransformTest(NULL),
  m_Saver(new QcepSettingsSaver(this)),
  m_Debug(m_Saver, this, "debug", 0, "Debug Level"),
  m_InputDataDescriptor(m_Saver, this, "inputData", "", "Input Data Descriptor"),
  m_OutputDataDescriptor(m_Saver, this, "outputData", "", "Output Data Descriptor"),
  m_OutputSliceDataDescriptor(m_Saver, this, "outputSliceData", "", "Output Slice Data Descriptor"),
  m_Halting(QcepSettingsSaverWPtr(), this, "halting", false, "Set to halt operation in progress"),
  m_InverseAvailable(m_Saver, this, "inverseAvailable", false, "Is inverse transform available?"),
  m_Progress(QcepSettingsSaverWPtr(), this, "progress", 0, "Progress completed"),
  m_ProgressLimit(QcepSettingsSaverWPtr(), this, "progressLimit", 0, "Progress limit"),
  m_DependenciesPath(m_Saver, this, "dependenciesPath", "", "Dependencies saved in"),
  m_SettingsPath(m_Saver, this, "settingsPath", "", "Settings saved in")
{
  QcepProperty::registerMetaTypes();
  CctwqtDoubleMatrix3x3Property::registerMetaTypes();
  CctwqtDoubleVector3DProperty::registerMetaTypes();
  CctwqtUnitCellProperty::registerMetaTypes();

  g_Saver = m_Saver;

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(onDebugChanged(int)));

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(doAboutToQuit()));
}

QcepSettingsSaverWPtr CctwqtApplication::saver() const
{
  return m_Saver;
}

void CctwqtApplication::onDebugChanged(int dbg)
{
  if (g_DebugLevel) {
    g_DebugLevel->setDebugLevel(dbg);
  }
}

void CctwqtApplication::initialize()
{
  m_Window                  = new CctwqtMainWindow(this);

//  QMainWindow *win = new QMainWindow();
//  win -> show();

  m_InputDataManagerThread  = new CctwqtDataFrameManagerThread(this);
  m_InputDataManagerThread  -> start();

  m_InputDataManager        = m_InputDataManagerThread->manager();
  m_InputData               = new CctwqtInputData(m_InputDataManager, this);
  m_InputData               -> allocateChunks();

  m_OutputDataManagerThread = new CctwqtDataFrameManagerThread(this);
  m_OutputDataManagerThread -> start();

  m_OutputDataManager       = m_OutputDataManagerThread->manager();
  m_OutputData              = new CctwqtOutputData(m_OutputDataManager, this);
  m_OutputData              -> allocateChunks();

  m_OutputSliceDataManagerThread  = new CctwqtDataFrameManagerThread(this);
  m_OutputSliceDataManagerThread  -> start();

  m_OutputSliceDataManager        = m_OutputSliceDataManagerThread->manager();
  m_OutputSliceData               = new CctwqtOutputSliceData(m_OutputSliceDataManager);
  m_OutputSliceData               ->initialize();

  m_Transform        = new CctwqtCrystalCoordinateTransform(this);
  m_Transformer      = new CctwqtTransformer(m_InputData,
                                             m_OutputData,
                                             m_Transform, 1, 1, 1, this);
  m_SliceTransform   = new CctwqtCrystalCoordinateTransform(this);
  m_SliceTransformer = new CctwqtTransformer(m_InputData,
                                             m_OutputSliceData,
                                             m_SliceTransform, 1, 1, 1, this);

  m_TransformTest    = new CctwTransformTest(this, NULL);

  m_ScriptEngine     = new CctwqtScriptEngine(this, NULL);

  m_ScriptEngine->globalObject().setProperty("inputDataManager", m_ScriptEngine->newQObject(m_InputDataManager));
  m_ScriptEngine->globalObject().setProperty("inputData", m_ScriptEngine->newQObject(m_InputData));
  m_ScriptEngine->globalObject().setProperty("outputDataManager", m_ScriptEngine->newQObject(m_OutputDataManager));
  m_ScriptEngine->globalObject().setProperty("outputData", m_ScriptEngine->newQObject(m_OutputData));
  m_ScriptEngine->globalObject().setProperty("outputSliceDataManager", m_ScriptEngine->newQObject(m_OutputSliceDataManager));
  m_ScriptEngine->globalObject().setProperty("outputSliceData", m_ScriptEngine->newQObject(m_OutputSliceData));
  m_ScriptEngine->globalObject().setProperty("transform", m_ScriptEngine->newQObject(m_Transform));
  m_ScriptEngine->globalObject().setProperty("sliceTransform", m_ScriptEngine->newQObject(m_SliceTransform));
  m_ScriptEngine->globalObject().setProperty("transformer", m_ScriptEngine->newQObject(m_Transformer));
  m_ScriptEngine->globalObject().setProperty("sliceTransformer", m_ScriptEngine->newQObject(m_SliceTransformer));
  m_ScriptEngine->globalObject().setProperty("test", m_ScriptEngine->newQObject(m_TransformTest));
  m_ScriptEngine->globalObject().setProperty("application", m_ScriptEngine->newQObject(this));
  m_ScriptEngine->globalObject().setProperty("globals", m_ScriptEngine->globalObject());

  readSettings();

  m_Saver->start();

  m_Window->show();
}

void CctwqtApplication::doAboutToQuit()
{
  writeSettings();
}

void CctwqtApplication::printMessage(QString msg, QDateTime dt)
{
  if (m_Window) {
    m_Window->printMessage(msg, dt);
  }
}

void CctwqtApplication::evaluateCommand(QString cmd)
{
  if (m_ScriptEngine) {
    QScriptValue val = m_ScriptEngine->evaluate(cmd);

    printMessage(tr("%1 -> %2").arg(cmd).arg(val.toString()));
  }
}

void CctwqtApplication::readSettings()
{
  QSettings settings("xray.aps.anl.gov", "cctw");

  readSettings(&settings);
}

void CctwqtApplication::readSettings(QString path)
{
  QSettings settings(path, QSettings::IniFormat);

  printMessage(tr("Reading settings from %1").arg(path));

  readSettings(&settings);

  set_SettingsPath(path);
}

void CctwqtApplication::readSettings(QSettings *settings)
{
  QcepProperty::readSettings(this, &staticMetaObject, "cctw", settings, true);

  if (m_InputDataManager) {
    m_InputDataManager->readSettings(settings, "inputDataManager");
  }

  if (m_InputData) {
    m_InputData->readSettings(settings, "inputData");
  }

  if (m_OutputDataManager) {
    m_OutputDataManager->readSettings(settings, "outputDataManager");
  }

  if (m_OutputData) {
    m_OutputData->readSettings(settings, "outputData");
  }

  if (m_OutputSliceDataManager) {
    m_OutputSliceDataManager->readSettings(settings, "outputSliceDataManager");
  }

  if (m_OutputSliceData) {
    m_OutputSliceData->readSettings(settings, "outputSliceData");
  }

  if (m_Transform) {
    m_Transform->readSettings(settings, "transform");
  }

  if (m_Transformer) {
    m_Transformer->readSettings(settings, "transformer");
  }

  if (m_SliceTransform) {
    m_SliceTransform->readSettings(settings, "sliceTransform");
  }

  if (m_SliceTransformer) {
    m_SliceTransformer->readSettings(settings, "sliceTransformer");
  }

  if (m_TransformTest) {
    m_TransformTest->readSettings(settings, "test");
  }
}

void CctwqtApplication::writeSettings()
{
  QSettings settings("xray.aps.anl.gov", "cctw");

  printMessage(tr("Writing default settings"));

  writeSettings(&settings);
}

void CctwqtApplication::writeSettings(QString path)
{
  QSettings settings(path, QSettings::IniFormat);

  printMessage(tr("Writing settings to %1").arg(path));

  set_SettingsPath(path);

  writeSettings(&settings);
}

void CctwqtApplication::writeSettings(QSettings *settings)
{
  QcepProperty::writeSettings(this, &staticMetaObject, "cctw", settings, true);

  if (m_InputDataManager) {
    m_InputDataManager->writeSettings(settings, "inputDataManager");
  }

  if (m_InputData) {
    m_InputData->writeSettings(settings, "inputData");
  }

  if (m_OutputDataManager) {
    m_OutputDataManager->writeSettings(settings, "outputDataManager");
  }

  if (m_OutputData) {
    m_OutputData->writeSettings(settings, "outputData");
  }

  if (m_OutputSliceDataManager) {
    m_OutputSliceDataManager->writeSettings(settings, "outputSliceDataManager");
  }

  if (m_OutputSliceData) {
    m_OutputSliceData->writeSettings(settings, "outputSliceData");
  }

  if (m_Transform) {
    m_Transform->writeSettings(settings, "transform");
  }

  if (m_Transformer) {
    m_Transformer->writeSettings(settings, "transformer");
  }

  if (m_SliceTransform) {
    m_SliceTransform->writeSettings(settings, "sliceTransform");
  }

  if (m_SliceTransformer) {
    m_SliceTransformer->writeSettings(settings, "sliceTransformer");
  }

  if (m_TransformTest) {
    m_TransformTest->writeSettings(settings, "test");
  }
}

void CctwqtApplication::calculateChunkDependencies(CctwIntVector3D idx)
{
  printMessage(tr("Calculate Chunk Dependencies for chunk [%1,%2,%3]").arg(idx.x()).arg(idx.y()).arg(idx.z()));

  CctwIntVector3D chStart = m_InputData->chunkStart(idx);
  CctwIntVector3D chSize  = m_InputData->chunkSize();
  CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());

  for (int z=0; z<chSize.z(); z++) {
    for (int y=0; y<chSize.y(); y++) {
      for (int x=0; x<chSize.x(); x++) {
        CctwDoubleVector3D index = dblStart+CctwDoubleVector3D(x,y,z);

        CctwDoubleVector3D coords = m_InputData->origin()+index*m_InputData->scale();

        CctwDoubleVector3D xfmcoord = m_Transform->forward(coords);

        CctwIntVector3D    pixels   = m_OutputData->toPixel(xfmcoord);

        if (m_OutputData->containsPixel(pixels)) {
          CctwIntVector3D    opchunk  = m_OutputData->findChunkIndexContaining(xfmcoord);

          m_InputData->addDependency(idx, opchunk);
          m_OutputData->addDependency(opchunk, idx);
        }
      }
    }
  }

  printMessage(tr("Finished Chunk Dependencies for chunk [%1,%2,%3]").arg(idx.x()).arg(idx.y()).arg(idx.z()));

  prop_Progress()->incValue(1);
}

void CctwqtApplication::calculateDependencies()
{
  m_InputData->clearDependencies();
  m_OutputData->clearDependencies();

  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          QtConcurrent::run(this, &CctwqtApplication::calculateChunkDependencies, idx);
//          calculateChunkDependencies(idx);
        }
      }
    }
  }
}

void CctwqtApplication::saveDependencies(QString path)
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  QFile aFile;

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          prop_Progress()->incValue(1);
        }
      }
    }
  }
}

void CctwqtApplication::loadDependencies(QString path)
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  QFile aFile;

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          prop_Progress()->incValue(1);
        }
      }
    }
  }
}

void CctwqtApplication::reportDependencies()
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);

  printMessage(tr("Check mapping"));

  for (int n=0; n<chunks.volume(); n++) {
    if (get_Halting()) {
      break;
    } else {
      CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);
      int nn = m_InputData->chunkNumberFromIndex(idx);

      if (nn != n) {
        printMessage(tr("Problem: %1 => [%2,%3,%4] => %5").arg(n).arg(idx.x()).arg(idx.y()).arg(idx.z()).arg(nn));
      }
    }
  }

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);
          int n = m_InputData->chunkNumberFromIndex(idx);
          CctwIntVector3D idx2 = m_InputData->chunkIndexFromNumber(n);

          if (idx != idx2) {
            printMessage(tr("Problem: [%1,%2,%3] => %4 => [%5,%6,%7]")
                         .arg(idx.x()).arg(idx.y()).arg(idx.z())
                         .arg(n)
                         .arg(idx2.x()).arg(idx2.y()).arg(idx2.z()));
          }
        }
      }
    }
  }

  printMessage(tr("Example transformations"));

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);
          CctwIntVector3D start = m_InputData->chunkStart(idx);
          CctwDoubleVector3D dblstart(start.x(), start.y(), start.z());

          CctwDoubleVector3D coords = m_InputData->origin()+dblstart*m_InputData->scale();
          CctwDoubleVector3D xfmcoord = m_Transform->forward(coords);

          CctwIntVector3D crdpixel = m_InputData->toPixel(coords);
          CctwIntVector3D xfmpixel = m_OutputData->toPixel(xfmcoord);

          bool ok = m_OutputData->containsPixel(xfmpixel);

          CctwIntVector3D ipchunk = m_InputData->findChunkIndexContaining(coords);
          CctwIntVector3D opchunk = m_OutputData->findChunkIndexContaining(xfmcoord);

          int inchnk = m_InputData->chunkNumberFromIndex(ipchunk);
          int opchnk = m_OutputData->chunkNumberFromIndex(opchunk);

          printMessage(tr("Chunk:[%1](%2,%3,%4) => [%5](%6,%7,%8)")
                       .arg(inchnk).arg(coords.x()).arg(coords.y()).arg(coords.z())
                       .arg(opchnk).arg(xfmcoord.x()).arg(xfmcoord.y()).arg(xfmcoord.z())
                       );

          printMessage(tr("Pixel:[%1,%2,%3] => [%4,%5,%6], ok:%7")
                       .arg(crdpixel.x()).arg(crdpixel.y()).arg(crdpixel.z())
                       .arg(xfmpixel.x()).arg(xfmpixel.y()).arg(xfmpixel.z()).arg(ok)
                       );
        }
      }
    }
  }

  printMessage(tr("Input Data Dependencies"));

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          CctwqtDataChunk *chunk = m_InputData -> chunk(idx);

          if (chunk) {
            chunk->reportDependencies();
          }
        }
      }
    }
  }
}
