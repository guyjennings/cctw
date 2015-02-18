#ifndef CCTWQTMAINWINDOW_H
#define CCTWQTMAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QDateTime>
#include "cctwapplication.h"
#include "cctwqtsetupimportdialog.h"
#include "cctwqtsetupoutputdialog.h"
#include "cctwqtsetuptransformdialog.h"
#include "cctwqtsetupcomparedialog.h"
#include "cctwqtsetupcheckimportdialog.h"
#include "cctwvector3d.h"
#include "cctwqttransformtester.h"

namespace Ui {
class CctwqtMainWindow;
}

class CctwApplication;
class QwtPlotCurve;
class QwtLegend;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QwtPlotZoomer;

class CctwqtMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit CctwqtMainWindow(CctwApplication *app, QWidget *parent = 0);
  ~CctwqtMainWindow();
  CctwApplication* cctwApplication();

public slots:
  void printLine(QString line);
  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void doEvaluateCommand();
#ifdef WANT_IMPORT_COMMANDS
  void doSetupImport();
  void doImport();
#endif
  void doExecuteScriptFile();
  void doBrowseInputFile();
  void doCheckDataFile(QString path);
  void doBrowseInputDataset(QString entry);
  void doCheckDataset(QString name);
  void doSetupOutput();
  void doSetupTransform();
  void doTransform();
  void doCheckTransform();
  void doDummyTransform();
  void doHalt();
  void doLoadSettings();
  void doSaveSettings();
  void doSaveDependencies();
  void doLoadDependencies();
  void reportInputDependencies();
  void reportOutputDependencies();
  void onProgressUpdate();
  void doAnalyzePEMetaData();
  void doAnalyzeSpecDataFile();
  void possiblyClose();
  void plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4);

  void doCompareHDF5();

#ifdef WANT_IMPORT_COMMANDS
  void doCheckImportedData();
  void updateImportImagePaths(QStringList p);
#endif

  void doBrowseProject();
  void doProjectInput();
  void doProjectOutput();

private:
  bool wantToClose();
  void closeEvent (QCloseEvent * event);
  void reportDependencies(CctwChunkedData *data, QString title);
public:
  Ui::CctwqtMainWindow       *ui;

private:
  CctwApplication            *m_Application;
  CctwqtTransformTester      *m_TransformTester;

#ifdef WANT_IMPORT_COMMANDS
  QPointer<CctwqtSetupImportDialog>      m_SetupImportDialog;
#endif
  QPointer<CctwqtSetupOutputDialog>      m_SetupOutputDialog;
  QPointer<CctwqtSetupTransformDialog>   m_SetupTransformDialog;
  QPointer<CctwqtSetupCheckImportDialog> m_SetupCheckImportDialog;
  QPointer<CctwqtSetupCompareDialog>     m_SetupCompareDialog;

  QwtLegend                     *m_Legend;
  QwtPlotPanner                 *m_Panner;
  QwtPlotMagnifier              *m_Magnifier;
  QwtPlotZoomer                 *m_Zoomer;
};

#endif // CCTWQTMAINWINDOW_H
