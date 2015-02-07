
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

int Cctwtcl_Parameters_Cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  if (objc == 1) {
    QString res = g_Application->settingsScript();

    Tcl_Obj *result = Tcl_NewStringObj(qPrintable(res), -1);
    Tcl_SetObjResult(interp, result);

    return TCL_OK;
  } else if (objc == 2) {
    const char *cmd = Tcl_GetString(objv[1]);

    QScriptValue val = g_Application->evaluate(cmd);

    QString res = val.toString();
    Tcl_Obj *result = Tcl_NewStringObj(qPrintable(res), -1);
    Tcl_SetObjResult(interp, result);

    return TCL_OK;
  } else {
    Tcl_SetResult(interp, "usage: cctw_parameters [params]", TCL_STATIC);
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
    Tcl_SetResult(interp, (char*) "input chunk is not present\n", TCL_STATIC);
    return TCL_ERROR;
  }

  // Currently unused
  // CctwIntVector3D cksz = chunkData->chunkSize();
  // int             cklen = cksz.volume();

  // Perform the transform
  QMap<int,CctwDataChunk*> outputChunks;

  g_Application->m_Transformer->transformChunkData(chunkIndex, chunkData, outputChunks);

  printf("products: %i\n", outputChunks.size());

  // Assemble the output list: A list of tuples
  // TODO: Study range of outputChunks.size() to figure out how to construct this list efficiently
  Tcl_Obj *result = Tcl_NewListObj(0, NULL);

  foreach (CctwDataChunk *outputChunk, outputChunks)
  {
    // Add each chunk as a tuple to the output: (index, data blob, weights blob, length)
    // A blob is a pair: (pointer, length)

    // Construct the Tcl objects
    Tcl_Obj *index = Tcl_NewIntObj(outputChunk->index());
    int blob_length = outputChunk->chunkSize().volume() * sizeof(CctwChunkedData::MergeDataType);
    Tcl_Obj *blob_length_tcl = Tcl_NewIntObj(blob_length);
    Tcl_Obj *data_pointer = Tcl_NewWideIntObj((Tcl_WideInt) outputChunk->dataPointer());
    Tcl_Obj * const blob_data_array[] = { data_pointer, blob_length_tcl };
    Tcl_Obj *blob_data  = Tcl_NewListObj(2, blob_data_array);
    Tcl_Obj *weights_pointer = Tcl_NewWideIntObj((Tcl_WideInt) outputChunk->weightsPointer());
    Tcl_Obj * const blob_weights_array[] = { weights_pointer, blob_length_tcl };
    Tcl_Obj *blob_weights  = Tcl_NewListObj(2, blob_weights_array);

    // Assemble the tuple
    Tcl_Obj * const tuple_array[] = { index, blob_data, blob_weights, blob_length_tcl };
    Tcl_Obj *tuple = Tcl_NewListObj(4, tuple_array);
    // Add the tuple to the output list
    Tcl_ListObjAppendElement(interp, result, tuple);

    // Clean up
    outputChunk->detachData();
    outputChunk->detachWeights();
    delete outputChunk;
  }

  g_Application->m_InputData->releaseChunk(chunkIndex);

  Tcl_SetObjResult(interp, result);
  return TCL_OK;
}

int Cctwtcl_Merge_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_merge <blob-list> -> null
  // merges a list of blobs into one or more output blobs
  // <blob-list> is a list of lists
  // { { <merge ID> <data-pointer> <weights-pointer> <length> } ... }
  // If merge IDs differ, data is merged into multiple output blobs

  TCL_ARGS(2, "usage: cctw_merge { { <merge ID> <data-pointer> <weights-pointer> <length> } ... }");

  int       rc;
  int       nblobs;
  Tcl_Obj **blobs;

  rc = Tcl_ListObjGetElements(interp, objv[1], &nblobs, &blobs);

  if (rc != TCL_OK) {
    Tcl_SetResult(interp, "Input argument should be a list\n", TCL_STATIC);
    return TCL_ERROR;
  }

  for (int i=0; i<nblobs; i++) {
    int       nargs;
    Tcl_Obj **blobargs;

    rc = Tcl_ListObjGetElements(interp, blobs[i], &nargs, &blobargs);

    if (rc != TCL_OK) {
      Tcl_SetResult(interp, "Input list element should also be a list\n", TCL_STATIC);
      return TCL_ERROR;
    }

    if (nargs < 4) {
      Tcl_SetResult(interp, "Input list element should be at least 4 elements long\n", TCL_STATIC);
      return TCL_ERROR;
    }

    int chunkId;
    Tcl_WideInt dataPointer, weightPointer;
    int blobLength;

    rc = Tcl_GetIntFromObj(interp, blobargs[0], &chunkId);

    if (rc != TCL_OK) {
      Tcl_SetResult(interp, "Blob ID should be an integer\n", TCL_STATIC);
      return TCL_ERROR;
    }

    rc = Tcl_GetWideIntFromObj(interp, blobargs[1], &dataPointer);

    if (rc != TCL_OK) {
      Tcl_SetResult(interp, "Data Pointer should be wide integer\n", TCL_STATIC);
      return TCL_ERROR;
    }

    rc = Tcl_GetWideIntFromObj(interp, blobargs[2], &weightPointer);

    if (rc != TCL_OK) {
      Tcl_SetResult(interp, "Weight Pointer should be wide integer\n", TCL_STATIC);
      return TCL_ERROR;
    }

    rc = Tcl_GetIntFromObj(interp, blobargs[3], &blobLength);

    if (rc != TCL_OK) {
      Tcl_SetResult(interp, "Blob length should be an integer\n", TCL_STATIC);
      return TCL_ERROR;
    }

    CctwDataChunk *opchk = g_Application->m_OutputData->chunk(chunkId);

    if (opchk) {
      opchk->mergeData((CctwChunkedData::MergeDataType*) dataPointer,
                       (CctwChunkedData::MergeDataType*) weightPointer,
                       blobLength/sizeof(CctwChunkedData::MergeDataType));
    } else {
      printf("Output chunk %d does not exist, skipped\n", chunkId);
    }
  }
}

int Cctwtcl_Output_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_output <id>
  // Output chunk <id> to the output file and deallocate the chunk storage

  TCL_ARGS(2, "usage: cctw_output <chunk-id>");

  int rc;
  int chunkId;

  rc = Tcl_GetIntFromObj(interp, objv[1], &chunkId);

  if (rc != TCL_OK) {
    Tcl_SetResult(interp, "chunk id should be an integer\n", TCL_STATIC);
    return TCL_ERROR;
  }

  CctwDataChunk *opchk = g_Application->m_OutputData->chunk(chunkId);

  if (opchk) {
    g_Application->m_OutputData->writeChunk(chunkId);
  }

  return TCL_OK;
}
