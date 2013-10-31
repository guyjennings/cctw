#include <QScriptValue>
#include <QString>
#include "cctwtcl_commands.h"
#include "cctwqtdebug.h"
#include "qcepdocumentationdictionary.h"
#include "cctwqtapplication.h"
#include "cctwcrystalcoordinateparameters.h"

static CctwqtApplication *g_Application = NULL;
static CctwCrystalCoordinateParameters *g_Parameters = NULL;

int Cctwtcl_Initialize()
{
  int nargs = 2;
  char* args[] = {"cctw", "-n", 0};

  g_DebugLevel            = QSharedPointer<CctwqtDebugDictionary>(new CctwqtDebugDictionary());
  gDocumentationDirectory = new QcepDocumentationDictionary();

  g_Application = new CctwqtApplication(nargs, args);
  g_Application -> initialize(nargs, args);

  g_Parameters  = new CctwCrystalCoordinateParameters(g_Application);

  return TCL_OK;
}

int Cctwtcl_Cmd             (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  if (objc >= 2) {
    const char *cmd = Tcl_GetString(objv[1]);

    QScriptValue val = g_Application->evaluate(cmd);

    QString valStr = val.toString();

    printf("%s -> %s\n", cmd, qPrintable(valStr));

    Tcl_Obj *result = Tcl_NewStringObj(qPrintable(valStr), -1);

//  Tcl_SetResult(interp, /* (char*) qPrintable(valStr) */ "hello", TCL_STATIC);

    Tcl_SetObjResult(interp, result);
  }

  return TCL_OK;
}

//int Cctwtcl_Parameters_Cmd  (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
//{
//  return TCL_OK;
//}

int Cctwtcl_Dependencies_Cmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
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

static CctwqtDataChunk* Cctwtcl_Input_Chunk_Argument(ClientData clientData, Tcl_Interp *interp, Tcl_Obj *obj)
{
  int       objc;
  Tcl_Obj** objv;

  if (Tcl_ListObjGetElements(interp, obj, &objc, &objv) == TCL_OK) {
    int     chunkNumber = 0;
    double *data = NULL;
    double *weight = NULL;
    int     dataSize = 0;
    int     weightSize = 0;

    switch (objc) {
    case 5: // {chunkid datachunk size weightchunk wtsize}
    {
      long w;

      if (Tcl_GetLongFromObj(interp, objv[3], &w) == TCL_OK &&
          Tcl_GetIntFromObj(interp, objv[4], &weightSize) == TCL_OK) {
        weight = (double*) w;
      } else {
        Tcl_SetResult(interp, "Bad input weight", TCL_STATIC);
        return NULL;
      }
    }     // drop through to handle first 3 args

    case 3: // {chunkid datachunk size}
    {
      long d;

      if (Tcl_GetLongFromObj(interp, objv[1], &d) == TCL_OK &&
          Tcl_GetIntFromObj(interp, objv[2], &dataSize) == TCL_OK) {
        data = (double*) d;
      } else {
        Tcl_SetResult(interp, "Bad inpyut data", TCL_STATIC);
        return NULL;
      }

      if (Tcl_GetIntFromObj(interp, objv[0], &chunkNumber)) {
        return g_Application->newInputChunk(chunkNumber, data, dataSize, weight, weightSize);
      } else {
        Tcl_SetResult(interp, "Bad input chunk number", TCL_STATIC);
      }
    }
      break;

    default:
      Tcl_SetResult(interp, "Bad input data chunk", TCL_STATIC);
      return NULL;
    }
  } else {
    Tcl_SetResult(interp, "Bad input data chunk argument", TCL_STATIC);
    return NULL;
  }
}

static CctwqtDataChunk* Cctwtcl_Intermediate_Chunk_Argument(ClientData clientData, Tcl_Interp *interp, const Tcl_Obj *obj)
{
}

static CctwqtDataChunk* Cctwtcl_Output_Chunk_Argument(ClientData clientData, Tcl_Interp *interp, const Tcl_Obj *obj)
{
}

int Cctwtcl_Transform_Cmd   (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return TCL_OK;
}

int Cctwtcl_Merge_Cmd       (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return TCL_OK;
}

int Cctwtcl_Normalize_Cmd   (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return TCL_OK;
}

int Cctwtcl_Chunk_Create_Cmd       (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  if (objc >= 2) {
    int size;
    if (Tcl_GetIntFromObj(interp, objv[1], &size) == TCL_OK) {
      double *buff = new double[size];

      Tcl_SetObjResult(interp, Tcl_NewLongObj((long) buff));

      return TCL_OK;
    }
  }

  return TCL_ERROR;
}


int Cctwtcl_Chunk_Delete_Cmd       (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  printf("objc = %d\n", objc);

  if (objc >= 2) {
    long buff;
    if (Tcl_GetLongFromObj(interp, objv[1], &buff) == TCL_OK) {
      double *pbuff = (double*) buff;

      printf("Deleting %p\n", pbuff);

      delete [] pbuff;

      return TCL_OK;
    }
  }

  return TCL_ERROR;
}

