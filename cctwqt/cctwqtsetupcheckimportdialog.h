#ifndef CCTWQTSETUPCHECKIMPORTDIALOG_H
#define CCTWQTSETUPCHECKIMPORTDIALOG_H

#include <QDialog>

class CctwImportData;
class CctwqtMainWindow;

namespace Ui {
class CctwqtSetupCheckImportDialog;
}

class CctwqtSetupCheckImportDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CctwqtSetupCheckImportDialog(CctwqtMainWindow *parent, CctwImportData *data);
  ~CctwqtSetupCheckImportDialog();

public slots:
  void accept();

private:
  void setCurrentDirectory(QString dir);

private:
  Ui::CctwqtSetupCheckImportDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwImportData     *m_Data;
};

#endif // CCTWQTSETUPCHECKIMPORTDIALOG_H
