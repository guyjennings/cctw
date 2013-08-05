#ifndef CCTWQTAPPLICATION_H
#define CCTWQTAPPLICATION_H

#include <QApplication>
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwqtoutputslicedata.h"
#include "cctwqtdataframemanager.h"
#include "cctwqtdataframemanagerthread.h"
#include "cctwqttransformer.h"
#include "cctwqtcrystalcoordinatetransform.h"

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
  CctwqtMainWindow                  *m_Window;
  CctwqtDataFrameManager            *m_InputDataManager;
  CctwqtDataFrameManagerThread      *m_InputDataManagerThread;
  CctwqtInputData                   *m_InputData;
  CctwqtDataFrameManager            *m_OutputDataManager;
  CctwqtDataFrameManagerThread      *m_OutputDataManagerThread;
  CctwqtOutputData                  *m_OutputData;
  CctwqtDataFrameManager            *m_OutputSliceDataManager;
  CctwqtDataFrameManagerThread      *m_OutputSliceDataManagerThread;
  CctwqtOutputSliceData             *m_OutputSliceData;
  CctwqtCrystalCoordinateTransform  *m_Transform;
  CctwqtTransformer                 *m_Transformer;
  CctwqtCrystalCoordinateTransform  *m_SliceTransform;
  CctwqtTransformer                 *m_SliceTransformer;
  CctwqtScriptEngine                *m_ScriptEngine;
};

#endif // CCTWQTAPPLICATION_H
