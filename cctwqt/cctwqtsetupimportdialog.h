#ifndef CCTWQTSETUPIMPORTDIALOG_H
#define CCTWQTSETUPIMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupImportDialog;
}

class CctwqtMainWindow;

class CctwqtSetupImportDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupImportDialog(CctwqtMainWindow *parent);
  ~CctwqtSetupImportDialog();

public slots:
  void accept();

private:
  Ui::CctwqtSetupImportDialog *ui;
  CctwqtMainWindow   *m_Window;
};

#endif // CCTWQTSETUPIMPORTDIALOG_H
