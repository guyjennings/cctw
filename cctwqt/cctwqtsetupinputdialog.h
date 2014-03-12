#ifndef CCTWQTSETUPINPUTDIALOG_H
#define CCTWQTSETUPINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupInputDialog;
}

class CctwqtMainWindow;
class CctwChunkedData;

class CctwqtSetupInputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupInputDialog(CctwqtMainWindow *parent, CctwChunkedData *data);
  ~CctwqtSetupInputDialog();

public slots:
  void accept();
  void doBrowseInputFile();
  void doCheckDataFile(QString path);
  void doBrowseInputDataset(int entry);
  void doCheckDataset(QString name);

private:
  Ui::CctwqtSetupInputDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwChunkedData    *m_Data;
};

#endif // CCTWQTSETUPINPUTDIALOG_H
