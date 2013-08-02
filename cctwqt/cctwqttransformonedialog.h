#ifndef CCTWQTTRANSFORMONEDIALOG_H
#define CCTWQTTRANSFORMONEDIALOG_H

#include <QDialog>

namespace Ui {
class CctwqtTransformOneDialog;
}

class CctwqtTransformOneDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit CctwqtTransformOneDialog(QWidget *parent = 0);
  ~CctwqtTransformOneDialog();
  
private:
  Ui::CctwqtTransformOneDialog *ui;
};

#endif // CCTWQTTRANSFORMONEDIALOG_H
