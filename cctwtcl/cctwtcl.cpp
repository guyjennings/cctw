#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cctwtcl_commands.h"
#include "cctwtcltiff.h"

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

  Cctwtcl_Initialize();

  return TCL_OK;
}
