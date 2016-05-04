#ifndef CCTWQTSETUPCOMPAREDIALOG_H
#define CCTWQTSETUPCOMPAREDIALOG_H

#include <QDialog>

#include "cctwcomparer-ptr.h"
class CctwqtMainWindow;

namespace Ui {
class CctwqtSetupCompareDialog;
}

class CctwqtSetupCompareDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CctwqtSetupCompareDialog(CctwqtMainWindow *parent, CctwComparerPtr data);
  ~CctwqtSetupCompareDialog();

public slots:
  void accept();
  void browse1();
  void browse2();

private:
  Ui::CctwqtSetupCompareDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwComparerPtr     m_Data;
};

#endif // CCTWQTSETUPCOMPAREDIALOG_H
