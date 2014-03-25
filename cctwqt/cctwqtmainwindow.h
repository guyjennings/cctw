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
#include "cctwqtsetupcomparedialog.h"
#include "cctwqtsetupcheckimportdialog.h"
#include "cctwvector3d.h"

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
  void doTransform();
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

  void doCompareHDF5();
  void doCheckImportedData();

  void updateImportImagePaths(QStringList p);
  void updateImportChunkSize(CctwIntVector3D sz);
  void updateInputHDF5ChunkSize(CctwIntVector3D sz);
  void updateOutputHDF5ChunkSize(CctwIntVector3D sz);

private:
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

private:
  Ui::CctwqtMainWindow       *ui;
  CctwApplication          *m_Application;

  QPointer<CctwqtSetupImportDialog>      m_SetupImportDialog;
  QPointer<CctwqtSetupInputDialog>       m_SetupInputDialog;
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
