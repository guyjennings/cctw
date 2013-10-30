/*
 * cctwtcltiff.h
 *
 *  Created on: Oct 29, 2013
 *      Author: wozniak
 */

#ifndef CCTWTCLTIFF_H
#define CCTWTCLTIFF_H

#include <stdbool.h>

#include <tcl.h>

int Cctwtcltiff_Read_Cmd(ClientData clientData,
                         Tcl_Interp *interp,
                         int objc, Tcl_Obj *const objv[]);

#endif
