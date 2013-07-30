#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include "cctwqtmainwindow.h"

class CctwqtApplication : public QApplication
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int argc, char *argv[]);
  
signals:
  
public slots:
  
private:
  CctwqtMainWindow *m_Window;
};

#endif // CCTWQTAPPLICATION_H
