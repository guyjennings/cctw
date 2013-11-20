#ifndef CCTWAPPLICATION_H
#define CCTWAPPLICATION_H

#include <QApplication>
#include <QDateTime>
#ifndef NO_GUI
#include "cctwqtmainwindow.h"
#endif
#include "cctwinputdata.h"
#include "cctwoutputdata.h"
#include "cctwoutputslicedata.h"
#include "cctwinputdataframemanager.h"
#include "cctwoutputdataframemanager.h"
#include "cctwtransformer.h"
#include "cctwcrystalcoordinateparameters.h"
#include "cctwcrystalcoordinatetransform.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepsettingssaver.h"
#include "qcepobjectnamer.h"
#include "cctwqtpeingresscommand.h"
#include "cctwdatablobs.h"

class CctwqtMainWindow;
class CctwScriptEngine;

#ifndef NO_GUI
class QwtPlot;
class QwtPlotCurve;
#endif

class CctwApplication
#ifdef NO_GUI
    : public QCoreApplication
#else
    : public QApplication
#endif
{
  Q_OBJECT
public:
  explicit CctwApplication(int &argc, char *argv[]);
  void initialize(int &argc, char *argv[]);

signals:

public slots:
  void printLine(QString line);
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

  void analyzePEMetaData(QString path);
  void analyzeSpecDataFile(QString path);

#ifndef NO_GUI
  void plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4);
#endif

  int inputChunkOffset(CctwIntVector3D index, CctwIntVector3D localcoords);

  CctwCrystalCoordinateParameters *parameters() const;

  void addWorkOutstanding(int amt);
  void workCompleted(int amt);
  void waitCompleted();
  int  workOutstanding();

  QcepIntList dependencies(int chunkId);
  QList<CctwIntVector3D> dependencies(int cx, int cy, int cz);

  CctwInputDataBlob*                input     (int chunkId, QString inputDataURL);
  QList<CctwIntermediateDataBlob*>  transform (int chunkId, CctwInputDataBlob *chunk);
  CctwIntermediateDataBlob*         merge     (int chunkId, CctwIntermediateDataBlob *chunk1, CctwIntermediateDataBlob *chunk2);
  CctwOutputDataBlob*               normalize (int chunkId, CctwIntermediateDataBlob *chunk);
  void                              output    (int chunkId, QString outputDataURL, CctwOutputDataBlob *chunk);
  void                              deleteBlob(int chunkId, CctwDataBlob *blob);

  int inputChunkCount();
  int outputChunkCount();

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
#ifndef NO_GUI
  CctwqtMainWindow                   *m_Window;
#endif
  CctwCrystalCoordinateParameters    *m_Parameters;
  CctwInputDataFrameManager          *m_InputDataManager;
  CctwInputData                      *m_InputData;
  CctwOutputDataFrameManager         *m_OutputDataManager;
  CctwOutputData                     *m_OutputData;
  CctwOutputDataFrameManager         *m_OutputSliceDataManager;
  CctwOutputSliceData                *m_OutputSliceData;
  CctwCrystalCoordinateTransform     *m_Transform;
  CctwTransformer                    *m_Transformer;
  CctwCrystalCoordinateTransform     *m_SliceTransform;
  CctwTransformer                    *m_SliceTransformer;
  CctwScriptEngine                   *m_ScriptEngine;
  CctwqtPEIngressCommand             *m_PEIngressCommand;
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

  Q_PROPERTY(QString specDataFilePath READ get_SpecDataFilePath WRITE set_SpecDataFilePath)
  QCEP_STRING_PROPERTY(SpecDataFilePath)
};

extern QcepSettingsSaverPtr g_Saver;

#endif // CCTWAPPLICATION_H