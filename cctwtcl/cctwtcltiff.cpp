
/*
 * cctwtcltiff.cpp
 *
 *  Created on: Oct 29, 2013
 *      Author: wozniak
 */

#include <assert.h>
#include <iostream>

#include <tiffio.h>
#include <tcl.h>

#include "cctwtcltiff.h"

using namespace std;

#define UNUSED __attribute__((unused))

static bool tiff_read(char* filename)
{
  TIFF *tif = TIFFOpen(filename, "r");
  if (tif == NULL)
    return false;

  int width, height;

  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

  cout << "wh: " << width << " " << height << endl;

  TIFFClose(tif);
  return true;
}

int Cctwtcltiff_Read_Cmd(UNUSED ClientData clientData,
                         UNUSED Tcl_Interp *interp,
                         int objc, Tcl_Obj *const objv[])
{
  assert(objc == 2);

  char *filename = Tcl_GetString(objv[1]);

  bool result = tiff_read(filename);
  if (!result)
    return TCL_ERROR;

  return TCL_OK;
}
