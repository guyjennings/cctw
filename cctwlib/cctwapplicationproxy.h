#ifndef CCTWAPPLICATIONPROXY_H
#define CCTWAPPLICATIONPROXY_H

#include "cctwobject.h"
#include "cctwapplication.h"

class CctwApplicationProxy : public CctwObject
{
public:
  CctwApplicationProxy(QString name, CctwApplication *app);

private:
  CctwApplication *m_Application;
};

#endif // CCTWAPPLICATIONPROXY_H
