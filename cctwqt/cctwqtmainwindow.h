#ifndef CCTWQTMAINWINDOW_H
#define CCTWQTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CctwqtMainWindow;
}

class CctwqtMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit CctwqtMainWindow(QWidget *parent = 0);
  ~CctwqtMainWindow();
  
private:
  Ui::CctwqtMainWindow *ui;
};

#endif // CCTWQTMAINWINDOW_H
