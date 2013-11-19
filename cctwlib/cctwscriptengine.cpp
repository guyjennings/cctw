#include "cctwscriptengine.h"
#include "cctwapplication.h"
#include "cctwintvector3dproperty.h"
#include "cctwdoublevector3dproperty.h"
#include "cctwdoublematrix3x3property.h"
#include "cctwunitcellproperty.h"

CctwScriptEngine::CctwScriptEngine(CctwApplication *app, QObject *parent) :
  QScriptEngine(parent),
  m_Application(app)
{
  globalObject().setProperty("cctw",  newQObject(m_Application, QScriptEngine::QtOwnership));
  globalObject().setProperty("print", newFunction(printFunc));
  globalObject().setProperty("wait",  newFunction(waitFunc));
  globalObject().setProperty("loadPreferences",  newFunction(loadPreferencesFunc));
  globalObject().setProperty("executeScriptFile",  newFunction(executeScriptFileFunc));

  qScriptRegisterMetaType(this, CctwDoubleVector3DProperty::toScriptValue, CctwDoubleVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwIntVector3DProperty::toScriptValue, CctwIntVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwDoubleMatrix3x3Property::toScriptValue, CctwDoubleMatrix3x3Property::fromScriptValue);
  qScriptRegisterMetaType(this, CctwUnitCellProperty::toScriptValue, CctwUnitCellProperty::fromScriptValue);
}

CctwApplication* CctwScriptEngine::application() const
{
  return m_Application;
}

QScriptValue CctwScriptEngine::printFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->printMessage(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::waitFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->wait(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::loadPreferencesFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->readSettings(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::executeScriptFileFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->executeScriptFile(msg);
    }
  }

  return QScriptValue(engine, "");
}
