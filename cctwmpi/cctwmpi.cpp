#include <stdio.h>
#include "cctwcrystalcoordinatetransform.h"
#include "cctwrotationtransform.h"
#include "cctwcompoundtransform.h"
#include "cctwtransformer.h"

#include "cctwdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwapplication.h"

#ifdef CCTW_WANT_MPI
#include <mpi.h>
#endif

int main(int argc, char *argv[])
{
  g_DebugLevel            = QSharedPointer<CctwDebug>(new CctwDebug());
  gDocumentationDirectory = new QcepDocumentationDictionary();

  CctwApplication *app = new CctwApplication(argc, argv);

  int rank = 0, size = -1;

#ifdef CCTW_WANT_MPI
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

  app->initialize(argc, argv);
  app->set_MpiRank(rank);
  app->set_MpiSize(size);

  printf("MPI process %d of %d\n", rank, size);

  int res = app->exec();

#ifdef CCTW_WANT_MPI
  MPI_Finalize();
#endif

  return res;
}
