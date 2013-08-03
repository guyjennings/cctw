#ifndef CCTWQTTRANSFORMONEDIALOG_H
#define CCTWQTTRANSFORMONEDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtTransformOneDialog;
}

class CctwqtMainWindow;

class CctwqtTransformOneDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtTransformOneDialog(CctwqtMainWindow *parent);
  ~CctwqtTransformOneDialog();
  
public slots:
  void accept();

private:
  Ui::CctwqtTransformOneDialog *ui;
};

#endif // CCTWQTTRANSFORMONEDIALOG_H
