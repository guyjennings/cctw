#include "cctwqtapplication.h"
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwqtoutputslicedata.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwtransformer.h"
#include <QScriptValue>
#include "cctwqtscriptengine.h"

CctwqtApplication::CctwqtApplication(int &argc, char *argv[]) :
  QApplication(argc, argv),
  m_Window(NULL),
  m_InputDataManager(NULL),
  m_InputData(NULL),
  m_OutputDataManager(NULL),
  m_OutputData(NULL),
  m_OutputSliceDataManager(NULL),
  m_OutputSliceData(NULL),
  m_Transform(NULL),
  m_Transformer(NULL),
  m_SliceTransform(NULL),
  m_SliceTransformer(NULL),
  m_ScriptEngine(NULL)
{
}

void CctwqtApplication::initialize()
{
  m_Window                  = new CctwqtMainWindow(this);

//  QMainWindow *win = new QMainWindow();
//  win -> show();

  m_InputDataManagerThread  = new CctwqtDataFrameManagerThread(this);
  m_InputDataManagerThread  -> start();

  m_InputDataManager        = m_InputDataManagerThread->manager();
  m_InputData               = new CctwqtInputData(m_InputDataManager);

  m_OutputDataManagerThread = new CctwqtDataFrameManagerThread(this);
  m_OutputDataManagerThread -> start();

  m_OutputDataManager       = m_OutputDataManagerThread->manager();
  m_OutputData              = new CctwqtOutputData(m_OutputDataManager);

  m_OutputSliceDataManagerThread  = new CctwqtDataFrameManagerThread(this);
  m_OutputSliceDataManagerThread  -> start();

  m_OutputSliceDataManager        = m_OutputSliceDataManagerThread->manager();
  m_OutputSliceData               = new CctwqtOutputSliceData(m_OutputSliceDataManager);

  m_Transform        = new CctwCrystalCoordinateTransform();
  m_Transformer      = new CctwTransformer(m_InputData, m_OutputData, m_Transform, 1, 1, 1, 0);
  m_SliceTransform   = new CctwCrystalCoordinateTransform();
  m_SliceTransformer = new CctwTransformer(m_InputData, m_OutputSliceData, m_SliceTransform, 1, 1, 1, 0);
  m_ScriptEngine     = new CctwqtScriptEngine(this, NULL);

  m_Window->show();
}

void CctwqtApplication::printMessage(QString msg)
{
  if (m_Window) {
    m_Window->printMessage(msg);
  }
}

void CctwqtApplication::evaluateCommand(QString cmd)
{
  if (m_ScriptEngine) {
    QScriptValue val = m_ScriptEngine->evaluate(cmd);

    printMessage(tr("%1 -> %2").arg(cmd).arg(val.toString()));
  }
}
