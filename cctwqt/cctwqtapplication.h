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

class CctwqtApplication : public QApplication
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int &argc, char *argv[]);
  void initialize();

signals:

public slots:
  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void evaluateCommand(QString cmd);

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

public:
  QcepSettingsSaverWPtr saver() const;

private slots:
  void onDebugChanged(int dbg);

private:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

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

public:
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
