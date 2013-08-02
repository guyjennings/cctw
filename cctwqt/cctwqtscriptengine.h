#ifndef CCTWQTSCRIPTENGINE_H
#define CCTWQTSCRIPTENGINE_H

#include <QScriptEngine>
class CctwqtApplication;

class CctwqtScriptEngine : public QScriptEngine
{
  Q_OBJECT
public:
  explicit CctwqtScriptEngine(CctwqtApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  CctwqtApplication* application() const;

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);

private:
  CctwqtApplication *m_Application;
};

#endif // CCTWQTSCRIPTENGINE_H
