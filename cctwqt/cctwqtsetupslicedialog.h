#ifndef CCTWQTSETUPSLICEDIALOG_H
#define CCTWQTSETUPSLICEDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupSliceDialog;
}

class CctwqtSetupSliceDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupSliceDialog(QWidget *parent = 0);
  ~CctwqtSetupSliceDialog();
  
private:
  Ui::CctwqtSetupSliceDialog *ui;
};

#endif // CCTWQTSETUPSLICEDIALOG_H
