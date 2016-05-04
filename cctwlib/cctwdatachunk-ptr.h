#ifndef CCTWDATACHUNKPTR_H
#define CCTWDATACHUNKPTR_H

#include <QSharedPointer>

class CctwDataChunk;

typedef QSharedPointer<CctwDataChunk> CctwDataChunkPtr;
typedef QWeakPointer<CctwDataChunk>   CctwDataChunkWPtr;

#endif // CCTWDATACHUNKPTR_H
