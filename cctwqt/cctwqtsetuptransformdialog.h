#ifndef CCTWQTSETUPTRANSFORMDIALOG_H
#define CCTWQTSETUPTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupTransformDialog;
}

class CctwqtMainWindow;
class CctwqtApplication;

class CctwqtSetupTransformDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupTransformDialog(CctwqtApplication *app, CctwqtMainWindow *parent);
  ~CctwqtSetupTransformDialog();
  
public slots:
  void accept();

private slots:
  void updateTwoTheta();
  void updateOmega();
  void updateChi();
  void updatePhi();

private:
  void copyParametersToDialog();
  void copyDialogToParameters();

  double rad2deg(double rad);
  double deg2rad(double deg);

private:
  Ui::CctwqtSetupTransformDialog *ui;
  CctwqtApplication              *m_Application;
};

#endif // CCTWQTSETUPTRANSFORMDIALOG_H
