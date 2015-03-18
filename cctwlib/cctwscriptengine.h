#ifndef CCTWQTSCRIPTENGINE_H
#define CCTWQTSCRIPTENGINE_H

#include <QScriptEngine>
class CctwApplication;

class CctwScriptEngine : public QScriptEngine
{
  Q_OBJECT
public:
  explicit CctwScriptEngine(CctwApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
  void checkForExceptions();

public:
  CctwApplication* application() const;

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue waitFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue loadPreferencesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue executeScriptFileFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue showHelpFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue showVersionFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setThreadsFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setInputDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setInputChunksFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setInputDatasetFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setMaskFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setAnglesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setOutputDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setOutputDimsFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setOutputChunksFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setOutputDatasetFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue transformFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setSubsetFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue dependenciesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue noDependenciesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue normalizationFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue compressionFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue inputProjectFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue outputProjectFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setProjectOutputFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue mergeInputFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue mergeOutputFunc(QScriptContext *context, QScriptEngine *engine);

//  typedef QObject *QObjectP;

//  static QScriptValue QObjectToScriptValue(QScriptEngine *engine, const QObjectP &object);
//  static void         QObjectFromScriptValue(const QScriptValue& value, QObjectP &object);

private:
  CctwApplication *m_Application;
};

#endif // CCTWQTSCRIPTENGINE_H
