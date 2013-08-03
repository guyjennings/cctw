#ifndef CCTWQTSETUPINPUTDIALOG_H
#define CCTWQTSETUPINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupInputDialog;
}

class CctwqtMainWindow;

class CctwqtSetupInputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupInputDialog(CctwqtMainWindow *parent);
  ~CctwqtSetupInputDialog();

public slots:
  void accept();

private:
  Ui::CctwqtSetupInputDialog *ui;
  CctwqtMainWindow   *m_Window;
};

#endif // CCTWQTSETUPINPUTDIALOG_H
