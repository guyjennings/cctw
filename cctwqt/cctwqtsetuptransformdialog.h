#ifndef CCTWQTSETUPTRANSFORMDIALOG_H
#define CCTWQTSETUPTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtSetupTransformDialog;
}

class CctwqtSetupTransformDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtSetupTransformDialog(QWidget *parent = 0);
  ~CctwqtSetupTransformDialog();
  
private:
  Ui::CctwqtSetupTransformDialog *ui;
};

#endif // CCTWQTSETUPTRANSFORMDIALOG_H
