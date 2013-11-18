#ifndef CCTWTCL_COMMANDS_H
#define CCTWTCL_COMMANDS_H

#include <tcl.h>

int Cctwtcl_Initialize();

int Cctwtcl_Cmd               (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Count_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Dependencies_Cmd  (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Input_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Transform_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Merge_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Normalize_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Output_Cmd        (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

int Cctwtcl_Delete_Cmd        (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Blob_Cmd          (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


#endif // CCTWTCL_COMMANDS_H
