#ifndef CCTWCOMPARERPTR_H
#define CCTWCOMPARERPTR_H

#include <QSharedPointer>

class CctwComparer;

typedef QSharedPointer<CctwComparer> CctwComparerPtr;
typedef QWeakPointer<CctwComparer>   CctwComparerWPtr;

#endif // CCTWCOMPARERPTR_H
