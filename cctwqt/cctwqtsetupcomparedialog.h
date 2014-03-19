#ifndef CCTWQTSETUPCOMPAREDIALOG_H
#define CCTWQTSETUPCOMPAREDIALOG_H

#include <QDialog>

class CctwComparer;
class CctwqtMainWindow;

namespace Ui {
class CctwqtSetupCompareDialog;
}

class CctwqtSetupCompareDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CctwqtSetupCompareDialog(CctwqtMainWindow *parent, CctwComparer *data);
  ~CctwqtSetupCompareDialog();

public slots:
  void accept();
  void browse1();
  void browse2();

private:
  Ui::CctwqtSetupCompareDialog *ui;
  CctwqtMainWindow   *m_Window;
  CctwComparer    *m_Data;
};

#endif // CCTWQTSETUPCOMPAREDIALOG_H
