
#include <stdio.h>

#include <QMap>
#include <QScriptValue>
#include <QString>
#include "cctwtcl_commands.h"
#include "cctwdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwapplication.h"
#include "cctwcrystalcoordinateparameters.h"

#include "cctwdatachunk.h"

#include "cctwtclutils.h"

static CctwApplication *g_Application = NULL;
//static CctwCrystalCoordinateParameters *g_Parameters = NULL;

#define UNUSED __attribute__((unused))

int Cctwtcl_Initialize()
{
  int nargs = 2;
  char* args[] = {"cctw", "-n", 0};

  if (getenv("VALGRIND") != NULL)
    // We are running under valgrind: do not buffer stdout
    // Avoids sequencing issues with stderr
    setvbuf(stdout, NULL, _IONBF, 0);

  g_DebugLevel            = QSharedPointer<CctwDebug>(new CctwDebug());
  gDocumentationDirectory = new QcepDocumentationDictionary();

  g_Application = new CctwApplication(nargs, args);
  g_Application -> initialize(nargs, args);

//  g_Parameters  = new CctwCrystalCoordinateParameters(g_Application);

  return TCL_OK;
}

int Cctwtcl_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  if (objc >= 2) {
    const char *cmd = Tcl_GetString(objv[1]);

    printf("evaluating: {%s}\n", cmd);

    QScriptValue val = g_Application->evaluate(cmd);

    QString valStr = val.toString();

    printf("%s -> %s\n", cmd, qPrintable(valStr));

    Tcl_Obj *result = Tcl_NewStringObj(qPrintable(valStr), -1);

//  Tcl_SetResult(interp, /* (char*) qPrintable(valStr) */ "hello", TCL_STATIC);

    Tcl_SetObjResult(interp, result);

    return TCL_OK;
  }
  return TCL_ERROR;
}

int Cctwtcl_Input_Cmd(ClientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_input <input_data_path> <input_chunk_id> --> <input_data_blob>
  // Read a blob of input data from the file system, perform any masking and normalization needed
  // returns a pair { <pointer> <length> }

  TCL_ARGS(4, "usage: cctw_input <input_path> <dataset> <chunkid>");

  int chunkId = -1;
  char *path    = Tcl_GetString(objv[1]);
  char *dataset = Tcl_GetString(objv[2]);

  int rc;
  rc = Tcl_GetIntFromObj(interp, objv[3], &chunkId);
  if (rc != TCL_OK) {
    printf("chunkid must be an integer!");
    return TCL_ERROR;
  }

  int chunkX = 128;
  int chunkY = 128;
  int chunkZ = 128;

  int length = chunkX*chunkY*chunkZ*sizeof(CctwChunkedData::MergeDataType);

  printf("readChunk()...\n");
  CctwChunkedData::MergeDataType *chunk =
      CctwChunkedData::readChunk(QString(path), QString(dataset), chunkId);
  printf("dataPointer()...\n");

  Tcl_Obj *res = Tcl_NewListObj(0, NULL);
  Tcl_ListObjAppendElement(interp, res, Tcl_NewWideIntObj((Tcl_WideInt)chunk));
  Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(length));
  Tcl_SetObjResult(interp, res);

  return TCL_OK;
}

int Cctwtcl_Transform_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_transform <input_data_pointer>  --> a list of outputs
  // Transform a blob of input data into a list of intermediate blobs
  // returns a list of triples { { <merge ID> <pointer> <length> }... }

  TCL_ARGS(3, "usage: cctw_transform <chunk ptr> <chunk id>");

  char* t = Tcl_GetString(objv[1]);
  printf("arg 1: %s\n", t);

  // Retrieve inputs from Tcl
  int rc;
  int chunkIndex;
  rc = Tcl_GetIntFromObj(interp, objv[1], &chunkIndex);
  if (rc != TCL_OK) {
    printf("chunk index must be an integer!\n");
    return TCL_ERROR;
  }
  Tcl_WideInt inputPtr;
  rc = Tcl_GetWideIntFromObj(interp, objv[2], &inputPtr);
  if (rc != TCL_OK) {
    printf("chunk ptr must be an integer!\n");
    return TCL_ERROR;
  }

  int chunkX = 128;
  int chunkY = 128;
  int chunkZ = 128;
  int length = chunkX*chunkY*chunkZ*sizeof(CctwChunkedData::MergeDataType);

  // Set up a ChunkedData
  QString chunkDataName = QString("chunkdata-%1").arg(chunkIndex);
  CctwChunkedData chunkData(g_Application, CctwIntVector3D(2048,2048,10),
                            CctwIntVector3D(chunkX, chunkY, chunkZ),
                            true, chunkDataName, NULL);

  // Set up the input DataChunk
  QString chunkName = QString("chunk-%1").arg(chunkIndex);
  CctwDataChunk dataChunk(&chunkData, chunkIndex, chunkName, NULL);
  dataChunk.setBuffer((void*) inputPtr);
  CctwIntVector3D chunkSize(chunkX, chunkY, chunkZ);
  dataChunk.setChunkSize(chunkSize);
  dataChunk.resetChunkStart();

  // Perform the transform
  QMap<int,CctwChunkedData::MergeDataType*> outputChunks;
  g_Application->m_Transformer->transformChunkData2(chunkIndex, &dataChunk, outputChunks);
  printf("products: %i\n", outputChunks.size());

  // Assemble the output Tcl objects
  Tcl_Obj *result          = Tcl_NewListObj(0, NULL);
  Tcl_Obj *outputIdsDict   = Tcl_NewDictObj();
  Tcl_Obj *outputBlobsDict = Tcl_NewDictObj();
  int counter = 0;
  for (QMap<int,CctwChunkedData::MergeDataType*> ::iterator i = outputChunks.begin(); i != outputChunks.end(); ++i)
  {
    int outputChunkId          = i.key();
    CctwChunkedData::MergeDataType* outputChunk = i.value();
    Tcl_DictObjPut(interp, outputIdsDict,   Tcl_NewIntObj(counter), Tcl_NewIntObj(outputChunkId));
    Tcl_Obj *blob = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewWideIntObj((Tcl_WideInt) outputChunk));
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewIntObj(length));
    Tcl_DictObjPut(interp, outputBlobsDict, Tcl_NewIntObj(counter), blob);
    counter++;
  }

//  int dummies = 3;
//  for (int i = 0; i < dummies; i++)
//  {
//    void *p = malloc(length);
//    memset(p, i+1, 1024);
//    Tcl_Obj *entry = Tcl_NewListObj(0, NULL);
//    Tcl_ListObjAppendElement(interp, entry, Tcl_NewIntObj(100+i+1));
//    Tcl_ListObjAppendElement(interp, entry, Tcl_NewWideIntObj((Tcl_WideInt) p));
//    Tcl_ListObjAppendElement(interp, entry, Tcl_NewIntObj(length));
//    Tcl_ListObjAppendElement(interp, result, entry);
//  }

  Tcl_ListObjAppendElement(interp, result, outputIdsDict);
  Tcl_ListObjAppendElement(interp, result, outputBlobsDict);
  Tcl_SetObjResult(interp, result);
  return TCL_OK;
}
