#include "cctwqtapplication.h"
#include "cctwqtmainwindow.h"

CctwqtApplication::CctwqtApplication(int argc, char *argv[]) :
  QApplication(argc, argv),
  m_Window(new CctwqtMainWindow())
{
  m_Window->show();
}
