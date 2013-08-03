#include "cctwqtapplication.h"
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwqtoutputslicedata.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwtransformer.h"
#include <QScriptValue>
#include "cctwqtscriptengine.h"

CctwqtApplication::CctwqtApplication(int argc, char *argv[]) :
  QApplication(argc, argv),
  m_Window(NULL),
  m_InputData(NULL),
  m_OutputData(NULL),
  m_Transform(NULL),
  m_Transformer(NULL),
  m_ScriptEngine(NULL)
{
}

void CctwqtApplication::initialize()
{
  m_Window        = new CctwqtMainWindow(this);
  m_InputData     = new CctwqtInputData(this);
  m_OutputData    = new CctwqtOutputData(this);
  m_OutputSliceData = new CctwqtOutputSliceData();
  m_Transform     = new CctwCrystalCoordinateTransform();
  m_Transformer   = new CctwTransformer(m_InputData, m_OutputData, m_Transform, 1, 1, 1, 0);
  m_ScriptEngine  = new CctwqtScriptEngine(this, NULL);

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
