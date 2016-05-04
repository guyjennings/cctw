#include "cctwapplicationproxy.h"

CctwApplicationProxy::CctwApplicationProxy(QString name, CctwApplication *app)
  : CctwObject(name, QcepObjectWPtr()),
    m_Application(app)
{
}
