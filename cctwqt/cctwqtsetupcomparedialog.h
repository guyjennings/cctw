#ifndef CCTWQTSETUPCOMPAREDIALOG_H
#define CCTWQTSETUPCOMPAREDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupCompareDialog;
}

class CctwqtSetupCompareDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CctwqtSetupCompareDialog(QWidget *parent = 0);
  ~CctwqtSetupCompareDialog();

private:
  Ui::CctwqtSetupCompareDialog *ui;
};

#endif // CCTWQTSETUPCOMPAREDIALOG_H
