#ifndef CCTWQTSETUPTRANSFORMDIALOG_H
#define CCTWQTSETUPTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupTransformDialog;
}

class CctwqtMainWindow;
class CctwApplication;

class CctwqtSetupTransformDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupTransformDialog(CctwApplication *app, CctwqtMainWindow *parent);
  ~CctwqtSetupTransformDialog();
  
public slots:
  void accept();

private:
  void copyParametersToDialog();
  void copyDialogToParameters();

  double rad2deg(double rad);
  double deg2rad(double deg);

private:
  Ui::CctwqtSetupTransformDialog *ui;
  CctwApplication              *m_Application;
};

#endif // CCTWQTSETUPTRANSFORMDIALOG_H
