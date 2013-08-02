#ifndef CCTWQTSETUPOUTPUTDIALOG_H
#define CCTWQTSETUPOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupOutputDialog;
}

class CctwqtSetupOutputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupOutputDialog(QWidget *parent = 0);
  ~CctwqtSetupOutputDialog();
  
private:
  Ui::CctwqtSetupOutputDialog *ui;
};

#endif // CCTWQTSETUPOUTPUTDIALOG_H
