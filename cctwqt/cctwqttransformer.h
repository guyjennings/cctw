#ifndef CCTWQTTRANSFORMER_H
#define CCTWQTTRANSFORMER_H

#include "../cctwtransformer.h"

class CctwqtApplication;

class CctwqtTransformer : public CctwTransformer
{
  Q_OBJECT
public:
  explicit CctwqtTransformer(CctwqtApplication *application,
                             CctwInputDataInterface *input,    // The input data
                             CctwOutputDataInterface *output,  // The output data
                             CctwTransformInterface *xform,    // The transform
                             int osx, int osy, int osz,
                             QObject *parent);
  
signals:
  
public slots:
  void transformChunk(int nx, int ny, int nz);
  void transform();
  void checkTransform();

private:
  void transformChunkNumber(int n);

private:
  CctwqtApplication *m_Application;
  QAtomicInt         m_MergeCounter;
};

#endif // CCTWQTTRANSFORMER_H
