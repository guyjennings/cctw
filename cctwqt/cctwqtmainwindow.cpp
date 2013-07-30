#include "cctwqtmainwindow.h"
#include "ui_cctwqtmainwindow.h"

CctwqtMainWindow::CctwqtMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CctwqtMainWindow)
{
  ui->setupUi(this);
}

CctwqtMainWindow::~CctwqtMainWindow()
{
  delete ui;
}
