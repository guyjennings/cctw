#ifndef CCTWQTMAINWINDOW_H
#define CCTWQTMAINWINDOW_H

#include <QMainWindow>
#include "cctwqtapplication.h"

namespace Ui {
class CctwqtMainWindow;
}

class CctwqtApplication;

class CctwqtMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit CctwqtMainWindow(CctwqtApplication *app, QWidget *parent = 0);
  ~CctwqtMainWindow();
  
public slots:
  void printMessage(QString msg);
  void doEvaluateCommand();

private:
  Ui::CctwqtMainWindow *ui;
  CctwqtApplication    *m_Application;
};

#endif // CCTWQTMAINWINDOW_H
