#include <stdio.h>

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

  return app->get_ExitStatus();
}
