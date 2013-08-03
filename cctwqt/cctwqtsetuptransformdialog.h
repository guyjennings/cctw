#ifndef CCTWQTSETUPTRANSFORMDIALOG_H
#define CCTWQTSETUPTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupTransformDialog;
}

class CctwqtMainWindow;

class CctwqtSetupTransformDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupTransformDialog(CctwqtMainWindow *parent);
  ~CctwqtSetupTransformDialog();
  
public slots:
  void accept();

private:
  Ui::CctwqtSetupTransformDialog *ui;
};

#endif // CCTWQTSETUPTRANSFORMDIALOG_H
