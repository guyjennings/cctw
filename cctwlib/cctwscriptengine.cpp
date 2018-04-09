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
  globalObject().setProperty("setMask", newFunction(setMaskFunc));
  globalObject().setProperty("setMask3D", newFunction(setMask3DFunc));
  globalObject().setProperty("setAngles", newFunction(setAnglesFunc));
  globalObject().setProperty("setOmega", newFunction(setOmegaFunc));
  globalObject().setProperty("setTwoTheta", newFunction(setTwoThetaFunc));
  globalObject().setProperty("setPhi", newFunction(setPhiFunc));
  globalObject().setProperty("setChi", newFunction(setChiFunc));
  globalObject().setProperty("setWeights", newFunction(setWeightsFunc));
  globalObject().setProperty("setOutputData",  newFunction(setOutputDataFunc));
  globalObject().setProperty("setOutputDims",  newFunction(setOutputDimsFunc));
  globalObject().setProperty("setOutputChunks",  newFunction(setOutputChunksFunc));
  globalObject().setProperty("setOutputDataset",  newFunction(setOutputDatasetFunc));
  globalObject().setProperty("setSubset", newFunction(setSubsetFunc));
  globalObject().setProperty("doTransform",  newFunction(transformFunc));
  globalObject().setProperty("normalization",  newFunction(normalizationFunc));
  globalObject().setProperty("compression",  newFunction(compressionFunc));
  globalObject().setProperty("inputProject",  newFunction(inputProjectFunc));
  globalObject().setProperty("outputProject",  newFunction(outputProjectFunc));
  globalObject().setProperty("setProjectOutput",  newFunction(setProjectOutputFunc));
  globalObject().setProperty("mergeInput",  newFunction(mergeInputFunc));
  globalObject().setProperty("mergeOutput",  newFunction(mergeOutputFunc));
  globalObject().setProperty("loadParameters", newFunction(loadParametersFunc));
  globalObject().setProperty("testCommand1", newFunction(testCommand1Func));
  globalObject().setProperty("testCommand2", newFunction(testCommand2Func));
  globalObject().setProperty("testCommand3", newFunction(testCommand3Func));

  qScriptRegisterMetaType(this, CctwDoubleVector3DProperty::toScriptValue, CctwDoubleVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwIntVector3DProperty::toScriptValue, CctwIntVector3DProperty::fromScriptValue);
  qScriptRegisterMetaType(this, CctwDoubleMatrix3x3Property::toScriptValue, CctwDoubleMatrix3x3Property::fromScriptValue);
  qScriptRegisterMetaType(this, CctwUnitCellProperty::toScriptValue, CctwUnitCellProperty::fromScriptValue);
//  qScriptRegisterMetaType(this, CctwScriptEngine::QObjectToScriptValue, CctwScriptEngine::QObjectFromScriptValue);

  qRegisterMetaType< QVector<int> >("QVector<int>");
  qRegisterMetaType< QVector<double> >("QVector<double>");

  qScriptRegisterSequenceMetaType< QVector<int> >(this);
  qScriptRegisterSequenceMetaType< QVector<double> >(this);
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

QScriptValue CctwScriptEngine::loadParametersFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->loadParameters(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::testCommand1Func(QScriptContext *context, QScriptEngine *engine)
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
      app->testCommand1(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::testCommand2Func(QScriptContext *context, QScriptEngine *engine)
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
      app->testCommand2(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::testCommand3Func(QScriptContext *context, QScriptEngine *engine)
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
      app->testCommand3(msg);
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

QScriptValue CctwScriptEngine::setMaskFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->set_MaskFile(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setMask3DFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setMask3DData(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setAnglesFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->set_AnglesFile(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setOmegaFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setOmega(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setTwoThetaFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setTwoTheta(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setChiFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setChi(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setPhiFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setPhi(msg);
    }
  }

  return QScriptValue(engine, "");
}


QScriptValue CctwScriptEngine::setWeightsFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->set_WeightsFile(msg);
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

QScriptValue CctwScriptEngine::transformFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->transform(msg);
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

QScriptValue CctwScriptEngine::compressionFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setCompression(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::inputProjectFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    int arg = 7;

    for (int i=0; i<nArgs; i++) {
      arg = context -> argument(i).toInteger();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->inputProject(arg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::outputProjectFunc(QScriptContext *context, QScriptEngine *engine)
{
  CctwScriptEngine *eng = qobject_cast<CctwScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    int arg = 7;

    for (int i=0; i<nArgs; i++) {
      arg = context -> argument(i).toInteger();
    }

    CctwApplication *app = eng->application();

    if (app) {
      app->outputProject(arg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::setProjectOutputFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->setProjectOutput(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::mergeInputFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->mergeInput(msg);
    }
  }

  return QScriptValue(engine, "");
}

QScriptValue CctwScriptEngine::mergeOutputFunc(QScriptContext *context, QScriptEngine *engine)
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
      app->mergeOutput(msg);
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
