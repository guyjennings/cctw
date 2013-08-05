#ifndef CCTWQTTRANSFORMER_H
#define CCTWQTTRANSFORMER_H

#include <QObject>
#include "cctwtransformer.h"

class CctwqtTransformer : public QObject, public CctwTransformer
{
  Q_OBJECT
public:
  explicit CctwqtTransformer(CctwInputDataInterface *input,    // The input data
                             CctwOutputDataInterface *output,  // The output data
                             CctwTransformInterface *xform,    // The transform
                             int osx, int osy, int osz,
                             QObject *parent = 0);
  
signals:
  
public slots:
  
};

#endif // CCTWQTTRANSFORMER_H
