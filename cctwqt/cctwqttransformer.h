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
  
};

#endif // CCTWQTTRANSFORMER_H
