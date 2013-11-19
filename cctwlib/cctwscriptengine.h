#ifndef CCTWQTSCRIPTENGINE_H
#define CCTWQTSCRIPTENGINE_H

#include <QScriptEngine>
class CctwqtApplication;

class CctwScriptEngine : public QScriptEngine
{
  Q_OBJECT
public:
  explicit CctwScriptEngine(CctwqtApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  CctwqtApplication* application() const;

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue waitFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue loadPreferencesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue executeScriptFileFunc(QScriptContext *context, QScriptEngine *engine);

private:
  CctwqtApplication *m_Application;
};

#endif // CCTWQTSCRIPTENGINE_H
