#ifndef CCTWAPPLICATIONPROXYPTR_H
#define CCTWAPPLICATIONPROXYPTR_H

#include <QSharedPointer>

class CctwApplicationProxy;

typedef QSharedPointer<CctwApplicationProxy> CctwApplicationProxyPtr;
typedef QWeakPointer<CctwApplicationProxy>   CctwApplicationProxyWPtr;

#endif // CCTWAPPLICATIONPROXYPTR_H
