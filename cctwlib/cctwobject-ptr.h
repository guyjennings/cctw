#ifndef CCTWOBJECTPTR_H
#define CCTWOBJECTPTR_H

#include <QSharedPointer>

class CctwObject;

typedef QSharedPointer<CctwObject> CctwObjectPtr;
typedef QWeakPointer<CctwObject>   CctwObjectWPtr;

#endif // CCTWOBJECTPTR_H
