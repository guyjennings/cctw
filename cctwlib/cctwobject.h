#ifndef CCTWQTOBJECT_H
#define CCTWQTOBJECT_H

#include <QObject>
#include <QSettings>
#include "qcepproperty.h"

class CctwObject : public QObject
{
  Q_OBJECT
public:
  explicit CctwObject(QObject *parent = 0);
  
signals:

public slots:
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

public:
  Q_PROPERTY(QString name READ get_Name WRITE set_Name)
  QCEP_STRING_PROPERTY(Name)
};

#endif // CCTWQTOBJECT_H
