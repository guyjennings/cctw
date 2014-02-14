#ifndef CCTWQTSETUPCOMPAREDIALOG_H
#define CCTWQTSETUPCOMPAREDIALOG_H

#include <QDialog>

class CctwCompareData;
class CctwqtMainWindow;

namespace Ui {
class CctwqtSetupCompareDialog;
}

class CctwqtSetupCompareDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CctwqtSetupCompareDialog(CctwqtMainWindow *parent, CctwCompareData *data);
  ~CctwqtSetupCompareDialog();

public slots:
  void accept();
  void browse1();
  void browse2();

private:
  Ui::CctwqtSetupCompareDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwCompareData    *m_Data;
};

#endif // CCTWQTSETUPCOMPAREDIALOG_H
