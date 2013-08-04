#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include "cctwqtmainwindow.h"
#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransformer.h"
#include "cctwqtdataframemanager.h"
#include "cctwqtdataframemanagerthread.h"

class CctwqtMainWindow;
class CctwqtScriptEngine;

class CctwqtApplication : public QApplication
{
  Q_OBJECT
public:
  explicit CctwqtApplication(int &argc, char *argv[]);
  void initialize();

signals:

public slots:
  void printMessage(QString msg);
  void evaluateCommand(QString cmd);

private:
  CctwqtMainWindow              *m_Window;
  CctwqtDataFrameManager        *m_InputDataManager;
  CctwqtDataFrameManagerThread  *m_InputDataManagerThread;
  CctwInputDataInterface        *m_InputData;
  CctwqtDataFrameManager        *m_OutputDataManager;
  CctwqtDataFrameManagerThread  *m_OutputDataManagerThread;
  CctwOutputDataInterface       *m_OutputData;
  CctwqtDataFrameManager        *m_OutputSliceDataManager;
  CctwqtDataFrameManagerThread  *m_OutputSliceDataManagerThread;
  CctwOutputDataInterface       *m_OutputSliceData;
  CctwTransformInterface        *m_Transform;
  CctwTransformer               *m_Transformer;
  CctwTransformInterface        *m_SliceTransform;
  CctwTransformer               *m_SliceTransformer;
  CctwqtScriptEngine            *m_ScriptEngine;
};

#endif // CCTWQTAPPLICATION_H
