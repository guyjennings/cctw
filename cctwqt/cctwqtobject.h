#ifndef CCTWQTOBJECT_H
#define CCTWQTOBJECT_H

#include <QObject>
#include <QSettings>

class CctwqtObject : public QObject
{
  Q_OBJECT
public:
  explicit CctwqtObject(QObject *parent = 0);
  
signals:

public slots:
  virtual void printMessage(QString msg);

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);
};

#endif // CCTWQTOBJECT_H
