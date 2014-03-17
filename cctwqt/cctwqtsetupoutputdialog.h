#ifndef CCTWQTSETUPOUTPUTDIALOG_H
#define CCTWQTSETUPOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupOutputDialog;
}

class CctwqtMainWindow;
class CctwChunkedData;

class CctwqtSetupOutputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupOutputDialog(CctwqtMainWindow *parent, CctwChunkedData *data);
  ~CctwqtSetupOutputDialog();
  
public slots:
  void accept();
  void doBrowseOutputFile();

private:
  Ui::CctwqtSetupOutputDialog *ui;
  CctwqtMainWindow            *m_Window;
  CctwChunkedData             *m_Data;
};

#endif // CCTWQTSETUPOUTPUTDIALOG_H
