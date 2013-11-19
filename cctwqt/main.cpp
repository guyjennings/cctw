#include <stdio.h>
#include "cctwcrystalcoordinatetransform.h"
#include "cctwrotationtransform.h"
#include "cctwcompoundtransform.h"
#include "cctwtransformer.h"
#include "cctwoutputchunkindex.h"

#include "cctwqtdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwqtapplication.h"

int main(int argc, char *argv[])
{
  g_DebugLevel            = QSharedPointer<CctwqtDebugDictionary>(new CctwqtDebugDictionary());
  gDocumentationDirectory = new QcepDocumentationDictionary();

  CctwqtApplication *app = new CctwqtApplication(argc, argv);

//  int appArgc = qApp->argc();

//  printf("appArgc = %d\n", appArgc);

  app->initialize(argc, argv);

  return app->exec();
}
