#ifndef CCTWQTMAINWINDOW_H
#define CCTWQTMAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QDateTime>
#include "cctwqtapplication.h"
#include "cctwqtsetupinputdialog.h"
#include "cctwqtsetupoutputdialog.h"
#include "cctwqtsetuptransformdialog.h"
#include "cctwqtsetupslicedialog.h"
#include "cctwqttransformonedialog.h"

namespace Ui {
class CctwqtMainWindow;
}

class CctwqtApplication;

class CctwqtMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit CctwqtMainWindow(CctwqtApplication *app, QWidget *parent = 0);
  ~CctwqtMainWindow();
  
public slots:
  void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  void doEvaluateCommand();
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
  void possiblyClose();

private:
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

private:
  Ui::CctwqtMainWindow       *ui;
  CctwqtApplication          *m_Application;

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
};

#endif // CCTWQTMAINWINDOW_H
