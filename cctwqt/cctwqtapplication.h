#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include <QDateTime>
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwqtoutputslicedata.h"
#include "cctwqtinputdataframemanager.h"
#include "cctwqtoutputdataframemanager.h"
#include "cctwqttransformer.h"
#include "cctwqtcrystalcoordinateparameters.h"
#include "cctwqtcrystalcoordinatetransform.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepsettingssaver.h"
#include "qcepobjectnamer.h"
#include "cctwtransformtest.h"

class CctwqtMainWindow;
class CctwqtScriptEngine;

class CctwqtApplication
#ifdef NO_GUI
    : public QCoreApplication
#else
    : public QApplication
#endif
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int &argc, char *argv[]);
  void initialize(int &argc, char *argv[]);

signals:

public slots:

  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void wait(QString msg);
  void evaluateCommand(QString cmd);
  void executeScriptFile(QString path);

  void writeSettings();
  void readSettings();
  void writeSettings(QString path);
  void readSettings(QString path);

  void calculateDependencies();
  void calculateChunkDependencies(CctwIntVector3D idx);

  void saveDependencies(QString path);
  void loadDependencies(QString path);

  void reportDependencies();

  void doAboutToQuit();

  void dummyInputRun();
  void dummyInputRunChunk(CctwIntVector3D idx);

  void reportInputDependencies();
  void reportInputDependencies(CctwIntVector3D idx);

  void reportOutputDependencies();
  void reportOutputDependencies(CctwIntVector3D idx);

  void reportInputChunkCounts();
  void reportOutputChunkCounts();

  int inputChunkOffset(CctwIntVector3D index, CctwIntVector3D localcoords);

  CctwqtCrystalCoordinateParameters *parameters() const;

  void addWorkOutstanding(int amt);
  void workCompleted(int amt);
  void waitCompleted();
  int  workOutstanding();

public:
  QcepSettingsSaverWPtr saver() const;

  QScriptValue evaluate(QString cmd);

private slots:
  void onDebugChanged(int dbg);
  void onProgress(int prg);

private:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void decodeCommandLineArgs(int &argc, char *argv[]);
  void decodeCommandLineArgsForUnix(int &argc, char *argv[]);
  void decodeCommandLineArgsForWindows(int &argc, char *argv[]);

public:
  QcepObjectNamer                     m_ObjectNamer;
  CctwqtMainWindow                   *m_Window;
  CctwqtCrystalCoordinateParameters  *m_Parameters;
  CctwqtInputDataFrameManager        *m_InputDataManager;
  CctwqtInputData                    *m_InputData;
  CctwqtOutputDataFrameManager       *m_OutputDataManager;
  CctwqtOutputData                   *m_OutputData;
  CctwqtOutputDataFrameManager       *m_OutputSliceDataManager;
  CctwqtOutputSliceData              *m_OutputSliceData;
  CctwqtCrystalCoordinateTransform   *m_Transform;
  CctwqtTransformer                  *m_Transformer;
  CctwqtCrystalCoordinateTransform   *m_SliceTransform;
  CctwqtTransformer                  *m_SliceTransformer;
  CctwqtScriptEngine                 *m_ScriptEngine;
  CctwTransformTest                  *m_TransformTest;
  QcepSettingsSaverPtr                m_Saver;

private:
  QAtomicInt                          m_DependencyCounter;
  QAtomicInt                          m_WorkOutstanding;
  QAtomicInt                          m_LastProgress;

public:
  Q_PROPERTY(bool guiWanted READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_BOOLEAN_PROPERTY(GuiWanted)

  Q_PROPERTY(QcepStringList startupCommands READ get_StartupCommands WRITE set_StartupCommands STORED false)
  QCEP_STRING_LIST_PROPERTY(StartupCommands)

  Q_PROPERTY(int debug READ get_Debug WRITE set_Debug)
  QCEP_INTEGER_PROPERTY(Debug)

  Q_PROPERTY(QString inputDataDescriptor READ get_InputDataDescriptor WRITE set_InputDataDescriptor)
  QCEP_STRING_PROPERTY(InputDataDescriptor)

  Q_PROPERTY(QString outputDataDescriptor READ get_OutputDataDescriptor WRITE set_OutputDataDescriptor)
  QCEP_STRING_PROPERTY(OutputDataDescriptor)

  Q_PROPERTY(QString outputSliceDataDescriptor READ get_OutputSliceDataDescriptor WRITE set_OutputSliceDataDescriptor)
  QCEP_STRING_PROPERTY(OutputSliceDataDescriptor)

  Q_PROPERTY(bool halting READ get_Halting WRITE set_Halting STORED false)
  QCEP_BOOLEAN_PROPERTY(Halting)

  Q_PROPERTY(bool inverseAvailable READ get_InverseAvailable WRITE set_InverseAvailable)
  QCEP_BOOLEAN_PROPERTY(InverseAvailable)

  Q_PROPERTY(int progress READ get_Progress WRITE set_Progress STORED false)
  QCEP_INTEGER_PROPERTY(Progress)

  Q_PROPERTY(int progressLimit READ get_ProgressLimit WRITE set_ProgressLimit STORED false)
  QCEP_INTEGER_PROPERTY(ProgressLimit)

  Q_PROPERTY(QString dependenciesPath READ get_DependenciesPath WRITE set_DependenciesPath)
  QCEP_STRING_PROPERTY(DependenciesPath)

  Q_PROPERTY(QString settingsPath READ get_SettingsPath WRITE set_SettingsPath)
  QCEP_STRING_PROPERTY(SettingsPath)
};

extern QcepSettingsSaverPtr g_Saver;

#endif // CCTWQTAPPLICATION_H
