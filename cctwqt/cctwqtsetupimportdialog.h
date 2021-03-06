#ifndef CCTWQTSETUPIMPORTDIALOG_H
#define CCTWQTSETUPIMPORTDIALOG_H

#include <QDialog>

class CctwImporter;
class CctwqtMainWindow;

namespace Ui {
class CctwqtSetupImportDialog;
}

class CctwqtSetupImportDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupImportDialog(CctwqtMainWindow *parent, CctwImporter *data);
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
  CctwImporter     *m_Data;
};

#endif // CCTWQTSETUPIMPORTDIALOG_H
