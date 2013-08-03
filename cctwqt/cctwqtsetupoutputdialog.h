#ifndef CCTWQTSETUPOUTPUTDIALOG_H
#define CCTWQTSETUPOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupOutputDialog;
}

class CctwqtMainWindow;

class CctwqtSetupOutputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupOutputDialog(CctwqtMainWindow *parent);
  ~CctwqtSetupOutputDialog();
  
public slots:
  void accept();

private:
  Ui::CctwqtSetupOutputDialog *ui;
};

#endif // CCTWQTSETUPOUTPUTDIALOG_H
