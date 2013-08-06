#include "cctwqtscriptengine.h"
#include "cctwqtapplication.h"

CctwqtScriptEngine::CctwqtScriptEngine(CctwqtApplication *app, QObject *parent) :
  QScriptEngine(parent),
  m_Application(app)
{
  globalObject().setProperty("cctw",  newQObject(m_Application, QScriptEngine::QtOwnership));
  globalObject().setProperty("print", newFunction(printFunc));
}

CctwqtApplication* CctwqtScriptEngine::application() const
{
  return m_Application;
}

QScriptValue CctwqtScriptEngine::printFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwqtScriptEngine *eng = qobject_cast<CctwqtScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    CctwqtApplication *app = eng->application();

    if (app) {
      app->printMessage(msg);
    }
  }

  return QScriptValue(engine, "");
}