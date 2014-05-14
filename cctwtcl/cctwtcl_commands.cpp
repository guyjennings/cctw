
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
  // cctw_input <input_chunk_id> --> <input_data_blob>
  // Read a blob of input data from the file system, perform any masking and normalization needed
  // returns a pair { <pointer> <length> }

  TCL_ARGS(2, "usage: cctw_input <chunkid>");

  int chunkId = -1;

  int rc;
  rc = Tcl_GetIntFromObj(interp, objv[1], &chunkId);
  if (rc != TCL_OK) {
    Tcl_SetResult(interp, "chunkid must be an integer!\n", TCL_STATIC);
    return TCL_ERROR;
  }

  CctwDataChunk *chunk = g_Application->m_InputData->readChunk(chunkId);

  if (chunk == NULL) {
    Tcl_SetResult(interp, "input chunk does not exist\n", TCL_STATIC);
    return TCL_ERROR;
  }

  int length = chunk->chunkSize().volume()*sizeof(CctwChunkedData::MergeDataType);

  Tcl_Obj *res = Tcl_NewListObj(0, NULL);
  Tcl_ListObjAppendElement(interp, res, Tcl_NewWideIntObj((Tcl_WideInt)chunk->dataPointer()));
  Tcl_ListObjAppendElement(interp, res, Tcl_NewWideIntObj((Tcl_WideInt)chunk->weightsPointer()));
  Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(length));

  chunk->detachData();
  chunk->detachWeights();

  Tcl_SetObjResult(interp, res);

  return TCL_OK;
}

int Cctwtcl_Transform_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_transform <input_data_pointer>  --> a list of outputs
  // Transform a blob of input data into a list of intermediate blobs
  // returns a list of lists { { <merge ID> <data-pointer> <weights-pointer> <length> }... }

  TCL_ARGS(2, "usage: cctw_transform <chunk id>");

  int rc;
  int chunkIndex;
  rc = Tcl_GetIntFromObj(interp, objv[1], &chunkIndex);
  if (rc != TCL_OK) {
    Tcl_SetResult(interp, "chunk index must be an integer!\n", TCL_STATIC);
    return TCL_ERROR;
  }

  CctwDataChunk *chunkData = g_Application->m_InputData->readChunk(chunkIndex);

  if (chunkData == NULL) {
    Tcl_SetResult(interp, "input chunk is not present\n", TCL_STATIC);
    return TCL_ERROR;
  }

  CctwIntVector3D cksz = chunkData->chunkSize();
  int             cklen = cksz.volume();

  // Perform the transform
  QMap<int,CctwDataChunk*> outputChunks;

  g_Application->m_Transformer->transformChunkData(chunkIndex, chunkData, outputChunks);

  printf("products: %i\n", outputChunks.size());

  // Assemble the output Tcl objects
  Tcl_Obj *result          = Tcl_NewListObj(0, NULL);

  foreach (CctwDataChunk *outputChunk, outputChunks)
  {
    Tcl_Obj *blob = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewIntObj(outputChunk->index()));
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewWideIntObj((Tcl_WideInt) outputChunk->dataPointer()));
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewWideIntObj((Tcl_WideInt) outputChunk->weightsPointer()));
    Tcl_ListObjAppendElement(interp, blob, Tcl_NewIntObj(outputChunk->chunkSize().volume()));

    outputChunk->detachData();
    outputChunk->detachWeights();

    delete outputChunk;

    Tcl_ListObjAppendElement(interp, result, blob);
  }

  Tcl_SetObjResult(interp, result);
  return TCL_OK;
}
