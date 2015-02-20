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
  globalObject().setProperty("showHelp",  newFunction(showHelpFunc));
  globalObject().setProperty("showVersion",  newFunction(showVersionFunc));
  globalObject().setProperty("setThreads",  newFunction(setThreadsFunc));
  globalObject().setProperty("setInputData",  newFunction(setInputDataFunc));
  globalObject().setProperty("setInputChunks",  newFunction(setInputChunksFunc));
  globalObject().setProperty("setInputDataset",  newFunction(setInputDatasetFunc));
  globalObject().setProperty("setMaskData", newFunction(setMaskDataFunc));
  globalObject().setProperty("setMaskDataset", newFunction(setMaskDatasetFunc));
  globalObject().setProperty("setAnglesData", newFunction(setAnglesDataFunc));
  globalObject().setProperty("setAnglesDataset", newFunction(setAnglesDatasetFunc));
  globalObject().setProperty("setOutputData",  newFunction(setOutputDataFunc));
  globalObject().setProperty("setOutputDims",  newFunction(setOutputDimsFunc));
  globalObject().setProperty("setOutputChunks",  newFunction(setOutputChunksFunc));
  globalObject().setProperty("setOutputDataset",  newFunction(setOutputDatasetFunc));
  globalObject().setProperty("setSubset", newFunction(setSubsetFunc));
  globalObject().setProperty("partialTransform",  newFunction(partialTransformFunc));
  globalObject().setProperty("partialDependencies",  newFunction(partialDependenciesFunc));
  globalObject().setProperty("noDependencies",  newFunction(noDependenciesFunc));
  globalObject().setProperty("normalization",  newFunction(normalizationFunc));

  qScriptRegisterMetaType(this, CctwDoubleVector3DProperty::toScriptValue, CctwDoubleVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwIntVector3DProperty::toScriptValue, CctwIntVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwDoubleMatrix3x3Property::toScriptValue, CctwDoubleMatrix3x3Property::fromScriptValue);
  qScriptRegisterMetaType(this, CctwUnitCellProperty::toScriptValue, CctwUnitCellProperty::fromScriptValue);
//  qScriptRegisterMetaType(this, CctwScriptEngine::QObjectToScriptValue, CctwScriptEngine::QObjectFromScriptValue);
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

QScriptValue CctwScriptEngine::setThreadsFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setThreads(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::showHelpFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->showHelp(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::showVersionFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->showVersion();
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setInputDataFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setInputData(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setInputChunksFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setInputChunks(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setInputDatasetFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setInputDataset(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setMaskDataFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setMaskData(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setMaskDatasetFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setMaskDataset(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setAnglesDataFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setAnglesData(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setAnglesDatasetFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setAnglesDataset(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setOutputDataFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setOutputData(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setOutputDimsFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setOutputDims(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setOutputChunksFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setOutputChunks(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setOutputDatasetFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setOutputDataset(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::partialTransformFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->partialTransform(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setSubsetFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setSubset(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::partialDependenciesFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->partialDependencies(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::noDependenciesFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->noDependencies();
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::normalizationFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setNormalization(msg);
    }
  }

  return QScriptValue(engine, "");
}

//QScriptValue CctwScriptEngine::QObjectToScriptValue(QScriptEngine *engine, const QObjectP &object)
//{
//  return engine->newQObject(object);
//}

//void         CctwScriptEngine::QObjectFromScriptValue(const QScriptValue& value, QObjectP &object)
//{
//  object = value.toQObject();
//}

void CctwScriptEngine::checkForExceptions()
{
  if (hasUncaughtException())
  {
    QScriptValue exception     = uncaughtException();
    int          exceptionLine = uncaughtExceptionLineNumber();

    if (m_Application) {
      m_Application -> printMessage(tr("Script resulted in exception!"));
      m_Application -> printMessage(tr("Line %1: %2").arg(exceptionLine).arg(exception.toString()));
    }
  }
}
