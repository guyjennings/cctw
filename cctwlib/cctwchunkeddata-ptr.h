#ifndef CCTWCHUNKEDDATAPTR_H
#define CCTWCHUNKEDDATAPTR_H

#include <QSharedPointer>

class CctwChunkedData;

typedef QSharedPointer<CctwChunkedData> CctwChunkedDataPtr;
typedef QWeakPointer<CctwChunkedData>   CctwChunkedDataWPtr;

#endif // CCTWCHUNKEDDATAPTR_H
