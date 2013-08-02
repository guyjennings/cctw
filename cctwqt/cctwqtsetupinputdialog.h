#ifndef CCTWQTSETUPINPUTDIALOG_H
#define CCTWQTSETUPINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupInputDialog;
}

class CctwqtSetupInputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupInputDialog(QWidget *parent = 0);
  ~CctwqtSetupInputDialog();
  
private:
  Ui::CctwqtSetupInputDialog *ui;
};

#endif // CCTWQTSETUPINPUTDIALOG_H
