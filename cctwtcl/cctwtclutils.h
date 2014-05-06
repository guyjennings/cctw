/*
 * cctwtclutils.h
 *
 *  Created on: Nov 18, 2013
 *      Author: wozniak
 */

#ifndef CCTWTCLUTILS_H
#define CCTWTCLUTILS_H

#include <tcl.h>

/**
   Check that the user gave us the correct number of arguments
   objc should be equal to count.  If not, fail.
   Note that in Tcl, the command name counts as an argument
*/
#define TCL_ARGS(count_expr, help) {                            \
    int count = count_expr;                                     \
    if (objc != count) {                                        \
      char* tmp = Tcl_GetStringFromObj(objv[0], NULL);          \
      printf("command %s requires %i arguments, received %i\n", \
             tmp, count, objc);                                 \
      printf(help "\n");                                        \
      return TCL_ERROR;                                         \
    }                                                           \
  }

/**
   Print error message and setup Tcl error state
 */
void tcl_condition_failed(Tcl_Interp* interp, Tcl_Obj* command,
                          const char* format, ...)
  __attribute__ ((format (printf, 3, 4)));

/**
   Print error message and return a Tcl error
   Requires Tcl_Interp interp and Tcl_Obj* objv in scope
 */
#define TCL_RETURN_ERROR(format, args...)                        \
  {                                                              \
    tcl_condition_failed(interp, objv[0], format, ## args);      \
    return TCL_ERROR;                                            \
  }

/**
   If rc is not TCL_OK, return a Tcl error with message
 */
#define TCL_CHECK_MSG(rc, format, args...)                        \
  if (rc != TCL_OK) {                                             \
    TCL_RETURN_ERROR(format, ## args);                            \
  }

/**
   If rc is not TCL_OK, return a blank Tcl error
 */
#define TCL_CHECK(rc) { if (rc != TCL_OK) { return TCL_ERROR; }}

#define TCL_CONDITION(condition, format, args...)                \
  if (!(condition)) {                                            \
    TCL_RETURN_ERROR(format, ## args);                           \
  }

#endif
