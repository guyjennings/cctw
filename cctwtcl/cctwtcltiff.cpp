
/*
 * cctwtcltiff.cpp
 *
 *  Created on: Oct 29, 2013
 *      Author: wozniak
 */

#include <assert.h>
#include <inttypes.h>
#include <iostream>
#include <stdlib.h>

#include <tiffio.h>
#include <tiffio.hxx>
#include <tcl.h>

#include "cctwtcltiff.h"

using namespace std;

#define UNUSED __attribute__((unused))

static bool tiff_read(char *filename,
                      void **output, uint *output_length)
{
  TIFF *tif = TIFFOpen(filename, "r");
  if (tif == NULL)
    return false;

  int width, height;

  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
  cout << "wh: " << width << " " << height << endl;

  int npixels = width * height;
  int length = npixels * sizeof (uint32);
  uint32* raster = (uint32*) malloc(length);
  assert(raster != NULL);

  int rc = TIFFReadRGBAImage(tif, width, height, raster, 0);
  assert(rc == 1);

  TIFFClose(tif);

  *output = raster;
  *output_length = length;
  return true;
}

int Cctwtcltiff_Read_Cmd(UNUSED ClientData clientData,
                         UNUSED Tcl_Interp *interp,
                         int objc, Tcl_Obj *const objv[])
{
  assert(objc == 4);

  char *filename   = Tcl_GetString(objv[1]);
  Tcl_Obj *pointer = objv[2];
  Tcl_Obj *length  = objv[3];

  void *output;
  uint bytes;
  bool rc = tiff_read(filename, &output, &bytes);
  if (!rc)
    return TCL_ERROR;

  Tcl_ObjSetVar2(interp, pointer, NULL, Tcl_NewWideIntObj((Tcl_WideInt)output), 0);
  Tcl_ObjSetVar2(interp, length,  NULL, Tcl_NewIntObj((int)bytes),      0);

  return TCL_OK;
}
