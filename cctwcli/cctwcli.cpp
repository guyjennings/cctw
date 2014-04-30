#include <stdio.h>
#include "cctwcrystalcoordinatetransform.h"
#include "cctwrotationtransform.h"
#include "cctwcompoundtransform.h"
#include "cctwtransformer.h"

#include "cctwdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwapplication.h"

int main(int argc, char *argv[])
{
  g_DebugLevel            = QSharedPointer<CctwDebug>(new CctwDebug());
  gDocumentationDirectory = new QcepDocumentationDictionary();

  CctwApplication *app = new CctwApplication(argc, argv);

  app->initialize(argc, argv);

  int res = app->exec();

  return res;
}
