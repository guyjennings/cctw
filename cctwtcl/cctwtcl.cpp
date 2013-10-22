#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cctwtcl_commands.h"

extern "C"
int Cctw_Init(Tcl_Interp *interp)
{
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

//  Tcl_CreateObjCommand(interp, "cctw_parameters", (Tcl_ObjCmdProc*) Cctwtcl_Parameters_Cmd,
//                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cctw_dependencies", (Tcl_ObjCmdProc*) Cctwtcl_Dependencies_Cmd,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cctw_transform", (Tcl_ObjCmdProc*) Cctwtcl_Transform_Cmd,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cctw_merge", (Tcl_ObjCmdProc*) Cctwtcl_Merge_Cmd,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);


  Cctwtcl_Initialize();

  return TCL_OK;
}
