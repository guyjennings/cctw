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
  return TCL_OK;
}

int Cctwtcl_Transform_Cmd   (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return TCL_OK;
}

int Cctwtcl_Merge_Cmd       (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return TCL_OK;
}

