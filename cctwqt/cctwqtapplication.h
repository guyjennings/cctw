#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include "cctwqtmainwindow.h"
#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransformer.h"

class CctwqtApplication : public QApplication
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int argc, char *argv[]);
  
signals:
  
public slots:
  
private:
  CctwqtMainWindow        *m_Window;
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwTransformInterface  *m_Transform;
  CctwTransformer         *m_Transformer;
};

#endif // CCTWQTAPPLICATION_H
