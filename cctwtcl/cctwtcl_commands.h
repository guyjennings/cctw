#ifndef CCTWTCL_COMMANDS_H
#define CCTWTCL_COMMANDS_H

#include <tcl.h>

int Cctwtcl_Initialize();

int Cctwtcl_Cmd               (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Input_Cmd         (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int Cctwtcl_Transform_Cmd     (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 *  Commands to implement a basic interface to cctw from swift/t via tcl
 *
 *  The following tcl commands are implemented:
 *
 *  cctw <cmd>
 *
 *    Passes javascript commands direct to the cctew script interpreter
 *
 *    Example:   cctw inputData.chunkSize
 *          -> 128,128,128
 *
 **/

#endif // CCTWTCL_COMMANDS_H
