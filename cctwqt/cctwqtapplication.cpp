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
  m_Saver(new QcepSettingsSaver(this)),
  m_Debug(m_Saver, this, "debug", 0, "Debug Level"),
  m_InputDataDescriptor(m_Saver, this, "inputData", "", "Input Data Descriptor"),
  m_OutputDataDescriptor(m_Saver, this, "outputData", "", "Output Data Descriptor"),
  m_OutputSliceDataDescriptor(m_Saver, this, "outputSliceData", "", "Output Slice Data Descriptor")
{
  QcepProperty::registerMetaTypes();

  g_Saver = m_Saver;

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(onDebugChanged(int)));
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

  m_OutputDataManagerThread = new CctwqtDataFrameManagerThread(this);
  m_OutputDataManagerThread -> start();

  m_OutputDataManager       = m_OutputDataManagerThread->manager();
  m_OutputData              = new CctwqtOutputData(m_OutputDataManager, this);

  m_OutputSliceDataManagerThread  = new CctwqtDataFrameManagerThread(this);
  m_OutputSliceDataManagerThread  -> start();

  m_OutputSliceDataManager        = m_OutputSliceDataManagerThread->manager();
  m_OutputSliceData               = new CctwqtOutputSliceData(m_OutputSliceDataManager);

  m_Transform        = new CctwqtCrystalCoordinateTransform(this);
  m_Transformer      = new CctwqtTransformer(m_InputData,
                                             m_OutputData,
                                             m_Transform, 1, 1, 1, this);
  m_SliceTransform   = new CctwqtCrystalCoordinateTransform(this);
  m_SliceTransformer = new CctwqtTransformer(m_InputData,
                                             m_OutputSliceData,
                                             m_SliceTransform, 1, 1, 1, this);
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

  readSettings();

  m_Saver->start();

  m_Window->show();
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
}
