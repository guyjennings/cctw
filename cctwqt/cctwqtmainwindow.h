#ifndef CCTWQTMAINWINDOW_H
#define CCTWQTMAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QDateTime>
#include "cctwapplication.h"
#include "cctwqtsetupimportdialog.h"
#include "cctwqtsetupinputdialog.h"
#include "cctwqtsetupoutputdialog.h"
#include "cctwqtsetuptransformdialog.h"
#include "cctwqtsetupslicedialog.h"
#include "cctwqttransformonedialog.h"

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
  
public slots:
  void printLine(QString line);
  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void doEvaluateCommand();
  void doSetupImport();
  void doImport();
  void doSetupInput();
  void doSetupOutput();
  void doSetupTransform();
  void doSetupSlice();
  void doTransformAll();
  void doTransformOne();
  void doTransformSlice();
  void doHalt();
  void doLoadSettings();
  void doSaveSettings();
  void doSaveDependencies();
  void doLoadDependencies();
  void onProgressUpdate();
  void doAnalyzePEMetaData();
  void doAnalyzeSpecDataFile();
  void possiblyClose();
  void plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4);

private:
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

private:
  Ui::CctwqtMainWindow       *ui;
  CctwApplication          *m_Application;

  QPointer<CctwqtSetupImportDialog>    m_SetupImportDialog;
  QPointer<CctwqtSetupInputDialog>     m_SetupInputDialog;
  QPointer<CctwqtSetupOutputDialog>    m_SetupOutputDialog;
  QPointer<CctwqtSetupTransformDialog> m_SetupTransformDialog;
  QPointer<CctwqtSetupSliceDialog>     m_SetupSliceDialog;
  QPointer<CctwqtTransformOneDialog>   m_TransformOneDialog;
//  CctwqtSetupInputDialog      *m_SetupInputDialog;
//  CctwqtSetupOutputDialog     *m_SetupOutputDialog;
//  CctwqtSetupTransformDialog  *m_SetupTransformDialog;
//  CctwqtSetupSliceDialog      *m_SetupSliceDialog;
//  CctwqtTransformOneDialog    *m_TransformOneDialog;
  QwtLegend                     *m_Legend;
  QwtPlotPanner                 *m_Panner;
  QwtPlotMagnifier              *m_Magnifier;
  QwtPlotZoomer                 *m_Zoomer;
};

#endif // CCTWQTMAINWINDOW_H
