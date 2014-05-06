
#include <tcl.h>

#include "cctwtcl_commands.h"

extern "C"
int Cctwtcl_Init(Tcl_Interp *interp)
{
  if (sizeof(void*) > sizeof(long)) {
    Tcl_SetResult(interp, (char*) "ERROR: Pointers are bigger than longs", TCL_STATIC);

    return TCL_ERROR;
  }

  if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
      return TCL_ERROR;
  }

  if (Tcl_PkgRequire(interp, "Tcl", "8.1", 0) == NULL) {
      return TCL_ERROR;
  }

  if (Tcl_PkgProvide(interp, "cctw", "0.1") != TCL_OK) {
      return TCL_ERROR;
  }

  Tcl_CreateObjCommand(interp, "cctw", (Tcl_ObjCmdProc *) Cctwtcl_Cmd,
          (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cctw_input", (Tcl_ObjCmdProc *) Cctwtcl_Input_Cmd,
          (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cctw_transform", (Tcl_ObjCmdProc *) Cctwtcl_Transform_Cmd,
          (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Cctwtcl_Initialize();

  return TCL_OK;
}
