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
  
public:
  CctwApplication* application() const;

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue waitFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue loadPreferencesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue executeScriptFileFunc(QScriptContext *context, QScriptEngine *engine);

private:
  CctwApplication *m_Application;
};

#endif // CCTWQTSCRIPTENGINE_H
