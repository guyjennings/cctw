#ifndef CCTWQTSETUPIMPORTDIALOG_H
#define CCTWQTSETUPIMPORTDIALOG_H

#include <QDialog>
#include "cctwimportdata.h"

namespace Ui {
class CctwqtSetupImportDialog;
}

class CctwqtMainWindow;

class CctwqtSetupImportDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupImportDialog(CctwqtMainWindow *parent, CctwImportData *data);
  ~CctwqtSetupImportDialog();

public slots:
  void accept();
  void doAppendImages();
  void doClearImages();
  void doBrowseDirectory();
  void doBrowseDirectoryChanged(int n);
  void doBrowseDark();
  void doClearDark();
  void doMatchImages();
  void doBrowseOutput();

private:
  QString currentDirectory();
  void setCurrentDirectory(QString dir);
  void appendDirectoryPaths(QString dir);

private:
  Ui::CctwqtSetupImportDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwImportData     *m_Data;
};

#endif // CCTWQTSETUPIMPORTDIALOG_H
