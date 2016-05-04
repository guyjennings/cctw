#ifndef CCTWAPPLICATION_H
#define CCTWAPPLICATION_H

#include <QApplication>
#include <QDateTime>
#ifndef NO_GUI
#include "cctwqtmainwindow.h"
#endif
#include "cctwchunkeddata.h"
#include "cctwtransformer.h"
#include "cctwcrystalcoordinateparameters.h"
#include "cctwcrystalcoordinatetransform.h"
#include "qcepobjectnamer.h"
#include "cctwpeingresscommand.h"

#ifdef WANT_IMPORT_COMMANDS
#include "cctwimporter.h"
#endif

#include "cctwcomparer-ptr.h"
#include "cctwapplicationproxy-ptr.h"

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

  CctwApplicationProxyWPtr proxy();

signals:

public slots:
  void printLine(QString line);
  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void wait(QString msg);
  void evaluateStartupCommand(QString cmd);
  void evaluateCommand(QString cmd);
  void executeScriptFile(QString path);

  void execute();
  void executeTransform();
  void executeMerge();
  void executeNorm();
  void executeProject();

  void showHelp(QString about);
  void showVersion();

  void setThreads(QString desc);

  void clearWriteSettings();
  void writeSettings();
  void readSettings();
  void writeSettings(QString path);
  void readSettings(QString path);

  QString settingsScript();

  void setInputData(QString data);
  void setInputChunks(QString data);
  void setInputDataset(QString data);

  void setMaskData(QString data);
  void setMaskDataset(QString data);

  void setMask3DData(QString data);

  void setAnglesData(QString data);
  void setAnglesDataset(QString data);

  void setOutputData(QString data);
  void setOutputDims(QString data);
  void setOutputChunks(QString data);
  void setOutputDataset(QString data);

  void setOmega(QString data);
  void setTwoTheta(QString data);
  void setChi(QString data);
  void setPhi(QString data);

  void setSubset(QString desc);
  void transform(QString desc);

  void mergeInput(QString path);
  void mergeOutput(QString path);

  void runTransform();
  void runMerge();
  void runNorm();
  void runProject();

  void setNormalization(QString data);
  void setCompression(QString data);

  void inputProject(int axes);
  void outputProject(int axes);
  void setProjectOutput(QString dir);

  void doAboutToQuit();

  void analyzePEMetaData(QString path);
  void analyzeSpecDataFile(QString path);

  CctwCrystalCoordinateParameters *parameters() const;

  void addWorkOutstanding(int amt);
  void workCompleted(int amt);
  void waitCompleted();
  int  workOutstanding();

  int inputChunkCount();
  int outputChunkCount();

  void testing();

public:
#ifndef NO_GUI
  void plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4);
#endif

  QScriptValue evaluate(QString cmd);
  void printHDF5errors();

  static QString addSlashes(QString str);

private slots:
  void onDebugChanged(int dbg);
  void onProgress(int prg);

private:
  void startupCommand(QString cmd);
  void preStartup();
  void postStartup();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void decodeCommandLineArgs(int &argc, char *argv[]);
  void decodeCommandLineArgsForUnix(int &argc, char *argv[]);
  void decodeCommandLineArgsForWindows(int &argc, char *argv[]);

  void installHDF5ErrorHandler();

  void pushInputFile(QString path);
  void autoChunkSizes();
  void autoOutputFile(QString suffix);

public:
  QcepObjectNamer                     m_ObjectNamer;
#ifndef NO_GUI
  CctwqtMainWindow                   *m_Window;
#endif
  CctwCrystalCoordinateParameters    *m_Parameters;
#ifdef WANT_IMPORT_COMMANDS
  CctwImporter                       *m_ImportData;
#endif
  CctwComparerPtr                     m_CompareData;
  CctwChunkedDataPtr                  m_InputData;
  CctwChunkedDataPtr                  m_OutputData;
  CctwCrystalCoordinateTransform     *m_Transform;
  CctwTransformer                    *m_Transformer;
  CctwScriptEngine                   *m_ScriptEngine;
  CctwPEIngressCommand               *m_PEIngressCommand;

private:
  QAtomicInt                          m_WorkOutstanding;
  QAtomicInt                          m_LastProgress;

  CctwApplicationProxyPtr             m_Proxy;

public:
  Q_PROPERTY(bool guiWanted READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_BOOLEAN_PROPERTY(GuiWanted)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode STORED false)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(QcepStringList startupCommands READ get_StartupCommands WRITE set_StartupCommands STORED false)
  QCEP_STRING_LIST_PROPERTY(StartupCommands)

  Q_PROPERTY(QcepStringList inputFiles READ get_InputFiles WRITE set_InputFiles STORED false)
  QCEP_STRING_LIST_PROPERTY(InputFiles)

  Q_PROPERTY(QString outputFile READ get_OutputFile WRITE set_OutputFile STORED false)
  QCEP_STRING_PROPERTY(OutputFile)

  Q_PROPERTY(QString maskFile READ get_MaskFile WRITE set_MaskFile STORED false)
  QCEP_STRING_PROPERTY(MaskFile)

  Q_PROPERTY(QString mask3DFile READ get_Mask3DFile WRITE set_Mask3DFile STORED false)
  QCEP_STRING_PROPERTY(Mask3DFile)

  Q_PROPERTY(QString anglesFile READ get_AnglesFile WRITE set_AnglesFile STORED false)
  QCEP_STRING_PROPERTY(AnglesFile)

  Q_PROPERTY(QString weightsFile READ get_WeightsFile WRITE set_WeightsFile STORED false)
  QCEP_STRING_PROPERTY(WeightsFile)

  Q_PROPERTY(int debug READ get_Debug WRITE set_Debug)
  QCEP_INTEGER_PROPERTY(Debug)

  Q_PROPERTY(bool halting READ get_Halting WRITE set_Halting STORED false)
  QCEP_BOOLEAN_PROPERTY(Halting)

  Q_PROPERTY(int progress READ get_Progress WRITE set_Progress STORED false)
  QCEP_INTEGER_PROPERTY(Progress)

  Q_PROPERTY(int progressLimit READ get_ProgressLimit WRITE set_ProgressLimit STORED false)
  QCEP_INTEGER_PROPERTY(ProgressLimit)

  Q_PROPERTY(QString settingsPath READ get_SettingsPath WRITE set_SettingsPath)
  QCEP_STRING_PROPERTY(SettingsPath)

  Q_PROPERTY(QString scriptPath READ get_ScriptPath WRITE set_ScriptPath)
  QCEP_STRING_PROPERTY(ScriptPath)

  Q_PROPERTY(QString specDataFilePath READ get_SpecDataFilePath WRITE set_SpecDataFilePath)
  QCEP_STRING_PROPERTY(SpecDataFilePath)

  Q_PROPERTY(int mpiRank READ get_MpiRank WRITE set_MpiRank STORED false)
  QCEP_INTEGER_PROPERTY(MpiRank)

  Q_PROPERTY(int mpiSize READ get_MpiSize WRITE set_MpiSize STORED false)
  QCEP_INTEGER_PROPERTY(MpiSize)

  Q_PROPERTY(int verbosity READ get_Verbosity WRITE set_Verbosity STORED false)
  QCEP_INTEGER_PROPERTY(Verbosity)

  Q_PROPERTY(int exitStatus READ get_ExitStatus WRITE set_ExitStatus STORED false)
  QCEP_INTEGER_PROPERTY(ExitStatus)
};

extern QcepSettingsSaverPtr g_Saver;

#endif // CCTWAPPLICATION_H
