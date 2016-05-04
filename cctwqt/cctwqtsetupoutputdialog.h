#ifndef CCTWQTSETUPOUTPUTDIALOG_H
#define CCTWQTSETUPOUTPUTDIALOG_H

#include <QDialog>
#include "cctwchunkeddata-ptr.h"

namespace Ui {
class CctwqtSetupOutputDialog;
}

class CctwqtMainWindow;

class CctwqtSetupOutputDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupOutputDialog(CctwqtMainWindow *parent, CctwChunkedDataPtr data);
  ~CctwqtSetupOutputDialog();
  
public slots:
  void accept();
  void doBrowseOutputFile();

private:
  Ui::CctwqtSetupOutputDialog *ui;
  CctwqtMainWindow            *m_Window;
  CctwChunkedDataPtr           m_Data;
};

#endif // CCTWQTSETUPOUTPUTDIALOG_H
