#ifndef CCTWQTSETUPSLICEDIALOG_H
#define CCTWQTSETUPSLICEDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupSliceDialog;
}

class CctwqtMainWindow;

class CctwqtSetupSliceDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupSliceDialog(CctwqtMainWindow *parent);
  ~CctwqtSetupSliceDialog();
  
public slots:
  void accept();

private:
  Ui::CctwqtSetupSliceDialog *ui;
};

#endif // CCTWQTSETUPSLICEDIALOG_H
