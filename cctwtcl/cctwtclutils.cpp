/*
 * cctwtclutils.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: wozniak
 */

#include "cctwtclutils.h"

#define TCL_CONDITION_MSG_MAX 1024

void tcl_condition_failed(Tcl_Interp* interp, Tcl_Obj* command,
                          const char* format, ...)
{
  va_list va;
  va_start(va,format);
  char buffer[TCL_CONDITION_MSG_MAX];
  char* commandname = Tcl_GetStringFromObj(command, NULL);
  char* p = &buffer[0];
  p += sprintf(p, "\n");
  p += sprintf(p, "error: ");
  p += sprintf(p, "%s: ", commandname);
  p += vsprintf(p, format, va);
  p += sprintf(p, "\n\n");
  va_end(va);
  Tcl_AddErrorInfo(interp, buffer);
}



