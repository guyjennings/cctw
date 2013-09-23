#ifndef CCTWQTTRANSFORMER_H
#define CCTWQTTRANSFORMER_H

#include "../cctwtransformer.h"

class CctwqtTransformer : public CctwTransformer
{
  Q_OBJECT
public:
  explicit CctwqtTransformer(CctwInputDataInterface *input,    // The input data
                             CctwOutputDataInterface *output,  // The output data
                             CctwTransformInterface *xform,    // The transform
                             int osx, int osy, int osz,
                             QObject *parent);
  
signals:
  
public slots:
  void transformChunk(int nx, int ny, int nz);
  void transform();

private:
  void transformChunkNumber(int n);
};

#endif // CCTWQTTRANSFORMER_H
