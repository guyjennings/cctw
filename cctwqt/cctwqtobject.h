#ifndef CCTWQTOBJECT_H
#define CCTWQTOBJECT_H

#include <QObject>

class CctwqtObject : public QObject
{
  Q_OBJECT
public:
  explicit CctwqtObject(QObject *parent = 0);
  
signals:

public slots:
  virtual void printMessage(QString msg);
};

#endif // CCTWQTOBJECT_H
