#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include "cctwqtmainwindow.h"
#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransformer.h"

class CctwqtMainWindow;
class CctwqtScriptEngine;

class CctwqtApplication : public QApplication
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int argc, char *argv[]);
  void initialize();

signals:

public slots:
  void printMessage(QString msg);
  void evaluateCommand(QString cmd);

private:
  CctwqtMainWindow        *m_Window;
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwOutputDataInterface *m_OutputSliceData;
  CctwTransformInterface  *m_Transform;
  CctwTransformer         *m_Transformer;
  CctwTransformInterface  *m_SliceTransform;
  CctwTransformer         *m_SliceTransformer;
  CctwqtScriptEngine      *m_ScriptEngine;
};

#endif // CCTWQTAPPLICATION_H
