#include <QScriptValue>
#include <QString>
#include "cctwtcl_commands.h"
#include "cctwdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwapplication.h"
#include "cctwcrystalcoordinateparameters.h"

#include "cctwtclutils.h"

static CctwApplication *g_Application = NULL;
//static CctwCrystalCoordinateParameters *g_Parameters = NULL;

#define UNUSED __attribute__((unused))

int Cctwtcl_Initialize()
{
  int nargs = 2;
  char* args[] = {"cctw", "-n", 0};

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

    QScriptValue val = g_Application->evaluate(cmd);

    QString valStr = val.toString();

//    printf("%s -> %s\n", cmd, qPrintable(valStr));

    Tcl_Obj *result = Tcl_NewStringObj(qPrintable(valStr), -1);

//  Tcl_SetResult(interp, /* (char*) qPrintable(valStr) */ "hello", TCL_STATIC);

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

//int Cctwtcl_Count_Cmd  (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const /*objv*/[])
//{
//  if (objc != 1) {
//    Tcl_SetResult(interp, "No parameters expected: usage: cctw_count", TCL_STATIC);
//    return TCL_ERROR;
//  } else {
//    Tcl_Obj *res = Tcl_NewListObj(0, NULL);

//    Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(g_Application->inputChunkCount()));
//    Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(g_Application->outputChunkCount()));

//    Tcl_SetObjResult(interp, res);
//  }

//  return TCL_OK;
//}

//int Cctwtcl_ijk2id_Cmd (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]) {
//  TCL_ARGS(7);

//  int max_x, max_y, max_z, x, y, z;
//  int rc;
//  rc = Tcl_GetIntFromObj(interp, objv[1], &max_x);
//  TCL_CHECK(rc);
//  rc = Tcl_GetIntFromObj(interp, objv[2], &max_y);
//  TCL_CHECK(rc);
//  rc = Tcl_GetIntFromObj(interp, objv[3], &max_z);
//  TCL_CHECK(rc);
//  rc = Tcl_GetIntFromObj(interp, objv[4], &x);
//  TCL_CHECK(rc);
//  rc = Tcl_GetIntFromObj(interp, objv[5], &y);
//  TCL_CHECK(rc);
//  rc = Tcl_GetIntFromObj(interp, objv[6], &z);
//  TCL_CHECK(rc);

//  int result = CctwTransformer::XYZtoID(max_x, max_y, max_z,
//                                        x, y, z);

//  Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
//  return TCL_OK;
//}

int Cctwtcl_Dependencies_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  if (objc == 2) {
    int chunkId;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) == TCL_OK) {
      QcepIntList deps = g_Application->dependencies(chunkId);

      Tcl_Obj *result = Tcl_NewListObj(0, NULL);

      foreach(int dep, deps) {
        Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(dep));
      }

      Tcl_SetObjResult(interp, result);

      return TCL_OK;
    }
  } else if (objc == 4) {
    int cx, cy, cz;

    if (Tcl_GetIntFromObj(interp, objv[1], &cx) == TCL_OK &&
        Tcl_GetIntFromObj(interp, objv[2], &cy) == TCL_OK &&
        Tcl_GetIntFromObj(interp, objv[3], &cz) == TCL_OK) {
      QList<CctwIntVector3D> deps = g_Application->dependencies(cx, cy, cz);

      Tcl_Obj *result = Tcl_NewListObj(0, NULL);

      foreach(CctwIntVector3D dep, deps) {
        Tcl_Obj *item = Tcl_NewListObj(0, NULL);

        Tcl_ListObjAppendElement(interp, item, Tcl_NewIntObj(dep.x()));
        Tcl_ListObjAppendElement(interp, item, Tcl_NewIntObj(dep.y()));
        Tcl_ListObjAppendElement(interp, item, Tcl_NewIntObj(dep.z()));

        Tcl_ListObjAppendElement(interp, result, item);
      }

      Tcl_SetObjResult(interp, result);

      return TCL_OK;
    }
  } else {
    Tcl_SetResult(interp, "Wrong number of arguments", TCL_STATIC);
  }

  return TCL_ERROR;
}

int Cctwtcl_Input_Cmd       (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_input <input_data_path> <input_chunk_id> --> <input_data_blob>
  // Read a blob of input data from the file system, perform any masking and normalization needed
  // returns a triple { <blobid> <length> <blob> }

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_input <input_path> <chunkid>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    char *path  = NULL;

    path  = Tcl_GetString(objv[1]);

    if (path==NULL) {
      return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwInputDataBlob* blob = g_Application->input(chunkId, path);

    Tcl_Obj *res = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(blob->blobID()));
    Tcl_ListObjAppendElement(interp, res, Tcl_NewIntObj(blob->blobLength()));
    Tcl_ListObjAppendElement(interp, res, Tcl_NewLongObj((long) blob));

    Tcl_SetObjResult(interp, res);
  }

  return TCL_OK;
}

int Cctwtcl_Transform_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_transform <input_chunk_id> <input_data_blob>  --> a list of <ouput_chunk_id> <intermediate_blob> pairs
  // Transform a blob of input data into a list of intermediate blobs
  // returns a list of triples { { <blobid> <length> <blob> }... }

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_transform <chunkid> <inputchunk>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long chunkP = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwInputDataBlob *blob = CctwInputDataBlob::validate(chunkId,chunkP);

    if (blob == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_transform", TCL_STATIC);
      return TCL_ERROR;
    }

    QList<CctwIntermediateDataBlob*> res = g_Application->transform(chunkId, blob);

    Tcl_Obj *result = Tcl_NewListObj(0, NULL);

    foreach(CctwIntermediateDataBlob* blob, res) {
      Tcl_Obj *item = Tcl_NewListObj(0, NULL);

      Tcl_ListObjAppendElement(interp, item, Tcl_NewIntObj(blob->blobID()));
      Tcl_ListObjAppendElement(interp, item, Tcl_NewIntObj(blob->blobLength()));
      Tcl_ListObjAppendElement(interp, item, Tcl_NewLongObj((long) blob));

      Tcl_ListObjAppendElement(interp, result, item);
    }

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

int Cctwtcl_Merge_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_merge <output_chunk_id> <intermediate_blob1> <intermediate_blob2> --> <intermediate_blob>
  // add together two intermediate blobs (with the same chunk_id) into a new intermediate blob
  // returns a triple { <chunkid> <length> <blob> }

  if (objc != 4) {
    Tcl_SetResult(interp, "Wrong number of parameters: usage: cctw_merge <chunkid> <blob1> <blob2>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long blob1P = 0, blob2P = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &blob1P) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[3], &blob2P) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwIntermediateDataBlob *blob1 = CctwIntermediateDataBlob::validate(chunkId, blob1P);

    if (blob1 == NULL) {
      Tcl_SetResult(interp, "Invalid blob1 passed to cctw_merge", TCL_STATIC);
      return TCL_ERROR;
    }

    CctwIntermediateDataBlob *blob2 = CctwIntermediateDataBlob::validate(chunkId, blob2P);

    if (blob2 == NULL) {
      Tcl_SetResult(interp, "Invalid blob2 passed to cctw_merge", TCL_STATIC);
      return TCL_ERROR;
    }

    CctwIntermediateDataBlob *blob = g_Application->merge(chunkId, blob1, blob2);

    Tcl_Obj *result = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobID()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobLength()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewLongObj((long) blob));

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

int Cctwtcl_Normalize_Cmd(ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[])
{
  // cctw_normalize <output_chunk_id> <intermediate_blob> --> <output_blob>
  // divide data by weight in an intermediate blob and produce an output blob
  // returns a triple { <chunkid> <length> <blob> }

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of parameters: usage: cctw_normalize <chunkid> <blob>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long chunkP = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwIntermediateDataBlob *chunk = CctwIntermediateDataBlob::validate(chunkId, chunkP);

    if (chunk == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_normalize", TCL_STATIC);
      return TCL_ERROR;
    }

    CctwOutputDataBlob *blob = g_Application->normalize(chunkId, chunk);

    Tcl_Obj *result = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobID()));
    Tcl_ListObjAppendElement(interp, result,  Tcl_NewIntObj(blob->blobLength()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewLongObj((long) blob));

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

int Cctwtcl_Output_Cmd      (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_output <output_data_path> <output_chunk_id> <output_blob>
  // write an output blob onto the file system

  if (objc != 4) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_output <path> <chunkid> <blob>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    char *path = NULL;
    long chunkP = 0;

    path = Tcl_GetString(objv[1]);

    if (path == NULL) {
      return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[3], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwOutputDataBlob *blob = CctwOutputDataBlob::validate(chunkId, chunkP);

    if (blob == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_output", TCL_STATIC);
      return TCL_ERROR;
    }

    g_Application->output(chunkId, path, blob);
  }

  return TCL_OK;
}

int Cctwtcl_Delete_Cmd       (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_delete <chunkid> <blob>
  // delete a blob

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_delete <chunkid> <blob>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long chunkP = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwDataBlob* blob = CctwDataBlob::validate(chunkId, chunkP);

    if (blob == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_delete", TCL_STATIC);
      return TCL_ERROR;
    }

    g_Application->deleteBlob(chunkId, (CctwDataBlob*) chunkP);
  }

  return TCL_OK;
}

int Cctwtcl_Blob_Cmd        (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_blob <chunkid> <blob>
  // convert a blob into a tcl byte array object
  // returns a triple { <chunkid> <length> <blob-bin>
  // <blob-bin> may be used with the tcl binary command

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_blob <chunkid> <blob>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long chunkP = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwDataBlob* blob = CctwDataBlob::validate(chunkId, chunkP);

    if (blob == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_blob", TCL_STATIC);
      return TCL_ERROR;
    }

    Tcl_Obj *result = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobID()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobLength()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewByteArrayObj((unsigned char*) blob, blob->blobLength()));

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

int Cctwtcl_Blob_Info_Cmd   (ClientData /*clientData*/, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  // cctw_blob_info <blobid> <blob>
  // prints summary info about a blob

  if (objc != 3) {
    Tcl_SetResult(interp, "Wrong number of arguments: usage: cctw_blob_into <blobid> <blob>", TCL_STATIC);
    return TCL_ERROR;
  } else {
    int chunkId = -1;
    long chunkP = 0;

    if (Tcl_GetIntFromObj(interp, objv[1], &chunkId) != TCL_OK) {
      return TCL_ERROR;
    }

    if (Tcl_GetLongFromObj(interp, objv[2], &chunkP) != TCL_OK) {
      return TCL_ERROR;
    }

    CctwDataBlob* blob = CctwDataBlob::validate(chunkId, chunkP);

    if (blob == NULL) {
      Tcl_SetResult(interp, "Invalid blob passed to cctw_blob_info", TCL_STATIC);
      return TCL_ERROR;
    }

    Tcl_Obj *result = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("type:", -1));

    switch (blob->blobType()) {
    case 0:
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("input", -1));
      break;
    case 1:
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("intermediate", -1));
      break;
    case 2:
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("output", -1));
      break;
    default:
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("unknown", -1));
      break;
    }

    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("id:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobID()));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("length:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(blob->blobLength()));

    int nw = 0, nnw = 0;
    int nmin = 0, nmax = 0;
    double min = 0, max = 0, maxw = 0;

    double *d = blob->data();
    double *w = blob->weight();

    int dl = blob->dataLength();
    int wl = blob->weightLength();

    if (w && wl) {
      for (int i=0; i<wl; i++) {
        if (w[i]) {
          nw  += 1;
          nnw += w[i];
        }
      }

      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("nw:", -1));
      Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(nw));
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("nnw:", -1));
      Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(nnw));
    }

    if (d && dl) {
      min = d[0]; max = d[0]; nmin = 0; nmax = 0;

      if (w && wl) {
        maxw = w[0];
      }

      for (int i=1; i<dl; i++) {
        if (d[i] > max) {
          max = d[i];
          nmax = i;

          if (w && wl) {
            maxw = w[i];
          }
        } else if (d[i] < min) {
          min = d[i];
          nmin = i;
        }
      }
    }

    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("min:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewDoubleObj(min));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("min@:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(nmin));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("max:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewDoubleObj(max));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("max@:", -1));
    Tcl_ListObjAppendElement(interp, result, Tcl_NewIntObj(nmax));

    if (w && wl) {
      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("maxw:", -1));
      Tcl_ListObjAppendElement(interp, result, Tcl_NewDoubleObj(maxw));
    }

    if (d) {
      Tcl_Obj *maxvs = Tcl_NewListObj(0, NULL);

      for (int i=nmax-5; i<nmax+5; i++) {
        if (i >= 0 && i<dl) {
          Tcl_ListObjAppendElement(interp, maxvs, Tcl_NewDoubleObj(d[i]));
        }
      }

      Tcl_ListObjAppendElement(interp, result, Tcl_NewStringObj("maxv:", -1));
      Tcl_ListObjAppendElement(interp, result, maxvs);
    }

    Tcl_SetObjResult(interp, result);

    return TCL_OK;
  }
}
